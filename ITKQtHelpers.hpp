/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef ITKQtHelpers_HPP
#define ITKQtHelpers_HPP

// ITK
#include "itkRegionOfInterestImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkVectorMagnitudeImageFilter.h"

// Custom
#include "Helpers/Helpers.h"
#include "ITKHelpers/ITKHelpers.h"

// Qt
#include <QColor>

namespace ITKQtHelpers
{

// template <typename TImage>
// QImage GetQImage(const TImage* image, const itk::ImageRegion<2>& region, const DisplayStyle& style)
// {
//   // Expects an itk::VectorImage
//   switch(style.Style)
//   {
//     case DisplayStyle::COLOR:
//       return GetQImageColor<TImage>(image, region);
//       break;
//     case DisplayStyle::MAGNITUDE:
//       return GetQImageMagnitude<TImage>(image, region);
//       break;
//     case DisplayStyle::CHANNEL:
//       {
//       typedef itk::Image<typename TImage::InternalPixelType, 2> ScalarImageType;
//       typename ScalarImageType::Pointer channelImage = ScalarImageType::New();
//       ITKHelpers::ExtractChannel<typename TImage::InternalPixelType>(image, style.Channel, channelImage);
//       return GetQImageScalar<ScalarImageType>(channelImage, region);
//       break;
//       }
//     default:
//       std::cerr << "No valid style specified." << std::endl;
//       return QImage();
//   }
//   return QImage();
// }


template <typename TImage>
QImage GetQImageColor(const TImage* const image)
{
  return GetQImageColor(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageColor(const TImage* const image, const itk::ImageRegion<2>& region)
{
  // Get a color QImage from an ITK image.
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage > RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();
    int r = static_cast<int>(pixel[0]);
    int g = static_cast<int>(pixel[1]);
    int b = static_cast<int>(pixel[2]);

    if(Helpers::IsValidRGB(r,g,b))
      {
      QColor pixelColor(r,g,b);
      qimage.setPixel(index[0], index[1], pixelColor.rgb());
      }
    else
      {
      std::cerr << "Can't set r,g,b to " << r << " " << g << " " << b << std::endl;
      QColor pixelColor(0,0,0);
      qimage.setPixel(index[0], index[1], pixelColor.rgb());
      }

    ++imageIterator;
    }


  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

template <typename TImage>
QImage GetQImageMagnitude(const TImage* image, const itk::ImageRegion<2>& region)
{
  // Get a QImage from a single channel ITK image. Each channel of the output image is the same.
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::Image<typename TImage::InternalPixelType, 2> ScalarImageType;
  typedef itk::VectorMagnitudeImageFilter<TImage, ScalarImageType>  VectorMagnitudeFilterType;
  typename VectorMagnitudeFilterType::Pointer magnitudeFilter = VectorMagnitudeFilterType::New();
  magnitudeFilter->SetInput(image);
  magnitudeFilter->Update();

  typedef itk::RescaleIntensityImageFilter<ScalarImageType, ITKHelpers::UnsignedCharScalarImageType> RescaleFilterType;
  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->SetInput( magnitudeFilter->GetOutput() );
  rescaleFilter->Update();

  typedef itk::RegionOfInterestImageFilter< ITKHelpers::UnsignedCharScalarImageType, ITKHelpers::UnsignedCharScalarImageType> RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(rescaleFilter->GetOutput());
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<ITKHelpers::UnsignedCharScalarImageType> imageIterator(regionOfInterestImageFilter->GetOutput(),
                                                                      regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    unsigned char pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue), static_cast<int>(pixelValue), static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

template <typename TImage>
QImage GetQImageChannel(const TImage* image, const itk::ImageRegion<2>& region, const unsigned int channel)
{
  typedef itk::Image<float, 2> ScalarImageType;
  typedef itk::VectorIndexSelectionCastImageFilter<TImage, ScalarImageType> IndexSelectionType;
  typename IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
  indexSelectionFilter->SetIndex(channel);
  indexSelectionFilter->SetInput(image);
  indexSelectionFilter->Update();

  return GetQImageScalar(indexSelectionFilter->GetOutput(), region);
}

template <typename TImage>
QImage GetQImageScalar(const TImage* image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage> RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(),
                                                 regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue), static_cast<int>(pixelValue), static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}


template <typename TImage>
QImage GetQImageRGBA(const TImage* const image)
{
  return GetQImageRGBA<TImage>(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageRGBA(const TImage* const image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_ARGB32);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage > RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();
    int r = static_cast<int>(pixel[0]);
    int g = static_cast<int>(pixel[1]);
    int b = static_cast<int>(pixel[2]);
    QColor pixelColor(r,g,b,255); // opaque
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
      {
      std::cout << "Can't set r,g,b to " << r << " " << g << " " << b << std::endl;
      }
    qimage.setPixel(index[0], index[1], pixelColor.rgba());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

} // end namespace

#endif

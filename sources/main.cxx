#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <itkImage.h>
//#include <itkImageFileReader.h>
//#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#include <itkImage.h>
#include <itkThresholdImageFilter.h>
#include <itkImageFileReader.h>



int main()
{
	int lowerThreshold = 10;
	int upperThreshold = 50;


	typedef signed short PixelType;
	typedef itk::Image<PixelType, 3> ImageType;
	typedef itk::ImageSeriesReader<ImageType> ReaderType;

	try
	{
		//ReaderType::Pointer reader = ReaderType::New();

		typedef itk::ImageFileWriter<ImageType> WriterType;
		typedef itk::ImageFileReader<ImageType> FileReaderType;
		FileReaderType::Pointer fileReader = FileReaderType::New();
		fileReader->SetFileName("D:/POMWJO/projekt/seria/im001.dcm");
		fileReader->Update();

		WriterType::Pointer fileWriter = WriterType::New();
		fileWriter->SetInput(fileReader->GetOutput());
		fileWriter->SetFileName("D:/POMWJO/projekt/wyniki/seria2/kopia3D.dcm");
		fileWriter->Update();

		typedef itk::Image<short, 3> ImgType3D;
		typedef itk::Image<short, 2> ImgType2D;
		typedef itk::ImageSeriesWriter<ImgType3D, ImgType2D> WriterType2D;

		WriterType2D::Pointer writer2D = WriterType2D::New();
		ImgType3D::Pointer image3 = fileReader->GetOutput();

		itk::NumericSeriesFileNames::Pointer namesGenerator2;
		namesGenerator2 = itk::NumericSeriesFileNames::New();
		namesGenerator2->SetSeriesFormat("D:/POMWJO/projekt/wyniki/seria2/IMG\%05d.dcm");
		namesGenerator2->SetStartIndex(1);
		namesGenerator2->SetEndIndex(image3->GetLargestPossibleRegion().GetSize()[2]);

		writer2D->SetFileNames(namesGenerator2->GetFileNames());
		writer2D->SetInput(image3);
		writer2D->Update();


		typedef itk::Image<unsigned char, 2>    ImageType;
		typedef itk::ImageFileReader<ImageType> ReaderType;
		
		ReaderType::Pointer reader = ReaderType::New();
		
		typedef itk::ThresholdImageFilter <ImageType> ThresholdImageFilterType;
		ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();

		reader->SetFileName("D:/POMWJO/projekt/wyniki/seria2/IMG00069.dcm");
		thresholdFilter->SetInput(reader->GetOutput());
		thresholdFilter->ThresholdOutside(lowerThreshold, upperThreshold);
		thresholdFilter->SetOutsideValue(0);
		reader->Update();

		typedef itk::ImageFileWriter<ImageType> FilteredWriter;
		FilteredWriter::Pointer writer = FilteredWriter::New();
		writer->SetInput(thresholdFilter->GetOutput());
		writer->SetFileName("D:/POMWJO/projekt/wyniki/seria3/wynik.dcm");
		writer->Update();
	}
	catch(itk::ExceptionObject &ex)
	{
	ex.Print(std::cout); 
	std::cin.get();
	return EXIT_FAILURE;
	}
	catch(std::exception &ex)
	{
	std::cout << ex.what() << std::endl;
	std::cin.get();
	return EXIT_FAILURE;
	}
	catch (...)
	{
	std::cout << "Nieokreslony blad!" << std::endl;
	std::cin.get();
	return EXIT_FAILURE;
	}
	
std::cin.get();
return EXIT_SUCCESS;
}
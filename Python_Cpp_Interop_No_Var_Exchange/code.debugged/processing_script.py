from imageprocessing import ImageDataHolder, ImageReader, ImageProcessor, ImageWriter

def main():
    dataHolder = ImageDataHolder()
    reader = ImageReader()
    processor = ImageProcessor()
    writer = ImageWriter()

    filename1 = input("Enter the name of the image to read: ")
    reader.read(filename1, dataHolder)

    print("Processing image...")
    processor.process(dataHolder)

    filename2 = input("Enter the name of the output image: ")
    writer.write(filename2, dataHolder)

    print("Done.")

if __name__ == "__main__":
    main()

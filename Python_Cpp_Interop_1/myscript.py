import libimageproc
import numpy

def main():
    reader = libimageproc.ImageReader()
    processor = libimageproc.ImageProcessor()
    writer = libimageproc.ImageWriter()

    input_filename = input("Enter the input filename: ")
    reader.read(input_filename)
    img = reader.img

    output_filename = input("Enter the output filename: ")
    processed_img = processor.process(img)
    writer.write(output_filename, processed_img)

if __name__ == "__main__":
    main()

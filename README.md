# Image Edit Application
This application has several filter applied. As a result user can preview the how each filter effects the image and save changed image into a new file.

## The usage of the application with sample image below
![Sample application usage screenshot](data/Image-Gallery-screenshot.jpg?raw=True)

## List of functionalities 
 1. Functional Filters applied:

 - conversion to greyscale
 - inverse colors
 - brightness correction
 - contrast correction
 - gamma correction
 - function filter using the box where user can change the curve of the function by adding new points to it

2. Convolutional Filters applied:

 - box blur
 - Gaussian blur
 - sharp
 - edge detection
 - emboss
 - median filter

3. Average Dithering - thresholding
4. Median Cut

### [Concept of dithering](https://www.tutorialspoint.com/dip/concept_of_dithering.htm)

### Dithering
K is the number colours in the output image. For K=2, this is a basic two level algorithm generating halftone images (black and white). Quantization of colouring occurs with this method.

### Median-Cut Algorithm
Subdivide the color space (e.g. the RGB cube) into K cuboids in such a way that each of them contains approximately the same amount of pixels. Each cuboid represents a color in the palette - either center of the cuboid or the average of pixels. I used the average approach.





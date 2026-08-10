### Phase I

- ~Single LED activation~
  - The 3x4 prototyped matrix is powered by 2 chained shift registers. A 16-bit binary value is fed to the registers, with the 4 MSBs currently irrelevant. Interfacing with each LED, with no abstraction, is as follows: RGBRGBRGBPPPPXXX.
- ~Verify row/column mapping~
- ~Row scanning implementation~

### Phase II

- Hardware timer refresh
- Monochrome framebuffer
- setPixel()

### Phase III

- Lines, rectangles, circles
- AxB font
- Text scrolling

### Phase IV

- PWM/BCM grayscale
- Performance optimization with SPI + DMA

### Phase V

- Resolution-independant graphics layer
- Image decoding

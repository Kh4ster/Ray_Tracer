Ray Tracer written in C++/CUDA

C++ was used to make a first working version and to allow for object oriented programming

Those C++ classes and codes got trimmed to allow the CUDA kernel to execute it. This is represented by the device classes.
Polymorphism could not be kept, so for the sake of the example, only Sphere was kept (we could do static polymorphism with a switch) and persecptive camera.
To allow for coalesced memory access the Color class is also change to have arrays of channels (RGB) and not an array of color.
Depth was reduced to avoid thread divergeance.

To go from host to device class we use convertion class. This is only used for code clarity and to allow to execute the code on both CUDA and C++. It would be much faster to use device code/class right away.
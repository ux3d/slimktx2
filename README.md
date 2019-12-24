# SlimKTX2
Slim C++ library for [KTX2](http://github.khronos.org/KTX-Specification/) binary parsing and writing - released under Apache License 2.0.

NOTE: SlimKTX2 is still a work in progress, APIs are not stable, bugs are hiding :)

This library is meant for binary parsing only, encoding and decoding compressed formats is left to the user. SlimKTX2 has no other external dependencies as it is written in plain C++11 without use of STL containers.

## Building

Use the provided CMakeLists to generate project files for your build system.

## Usages

Note: `Data Format Descriptor`, `Key/Value Data` and `Supercompression Global Data` are currently ignored / not handled.

### Callbacks

SlimKTX2 uses callbacks to allow custom behaviour for allocation and IO.
```cpp
void* allocate(void* _pUserData, size_t _size)
{
	return malloc(_size);
}

...

ux3d::slimktx2::Callbacks callbacks{};

callbacks.allocate = allocate;
callbacks.free = ...
```

Mandatory callbacks:
* `allocate` like `malloc()`
* `deallocate` like `free()`
* `tell` like `ftell()`

Callbacks for reading: `read` like `fread()` 
Callback for writing: `write` like `fwrite()`

Optinal for logging and error reporting:  `log` like `printf()`

### Parsing KTX2 files

First, setup callbacks required for reading and set them with `setCallbacks()`:

```cpp
using namespace ux3d::slimktx2;
// setup callbacks
Callbacks callbacks{};
callbacks.allocate = allocate;
callbacks.free = deallocate;
callbacks.read = readFromFile;
callbacks.tell = tell;

SlimKTX2 slimKTX2;
slimKTX2.setCallbacks(callbacks);

FILE* pFile = fopen("myfile.ktx2", "rb");

if (slimKTX2.parse(pFile) != Result::Success)
{
	fclose(pFile);
	return;
}

for(uint32_t level = 0; level < slimKTX2.getLevelCount(); ++level)
{
    for(uint32_t layer = 0; layer < slimKTX2.getLayerCount(); ++layer)
    {
        for(uint32_t face = 0; face < slimKTX2.getFaceCount(); ++face)
        {
            uint8_t* pDst = nullptr;
            if (slimKTX2.getImage(pDst, level, face, layer)) == Result::Success)
            {
                *pDst = 0xFF; // manipulate image data here
            }
        }
    }
}

fclose(pFile);
```

### Writing KTX2 files

First setup callbacks as before, now with `write` assigned.

```cpp
using namespace ux3d::slimktx2;
// setup callbacks
Callbacks callbacks{};
callbacks.write = writeToFile;
...

// init slim ktx callbacks via constructor
SlimKTX2 slimKTX2(callbacks);

// first specify the format of a half-float 1024x1024 cubemap (6 faces) with 5 mip levels:
slimKTX2.specifyFormat(Format::R16G16B16A16_SFLOAT, 1024, 1024, 5, 6);

// allocate memory for all levels, layers and faces
if(slimKTX2.allocateContainer() == Result::Success)
{
    // write some data to level 1, face +Y, layer 0
    // Cubemap faces are stored in the order: +X, -X, +Y, -Y, +Z, -Z.
    slimKTX2.setImage(_inData.data(), _inData.size(), 1, CubeMapFace::Top, 0);
    ...

    FILE* pFile = fopen("myfile.ktx2", "wb");
    if (slimKTX2.serialize(pFile) == Result::Success)
    {
        // thats it
    }
    fclose(pFile);
}
```
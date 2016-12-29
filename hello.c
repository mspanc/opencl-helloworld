/**
 * OpenCL Hello World
 *
 * On Mac OS X Sierra compile with
 * clang  -arch x86_64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -framework OpenCL hello.c
 */

#include <stdio.h>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#define PLATFORM_NAME_LEN 1024
#define PLATFORM_VENDOR_LEN 1024
#define DEVICE_NAME_LEN 1024
#define DEVICE_VENDOR_LEN 1024
#define DEVICE_VERSION_LEN 1024


int main()
{
  cl_int          error = CL_SUCCESS;
  cl_uint         platform_count = 0;
  cl_uint         device_count = 0;
  char            platform_name[PLATFORM_NAME_LEN];
  char            platform_vendor[PLATFORM_VENDOR_LEN];
  char            device_name[DEVICE_NAME_LEN];
  char            device_vendor[DEVICE_VENDOR_LEN];
  char            device_version[DEVICE_VERSION_LEN];
  cl_device_type  device_type;
  cl_platform_id* platform_ids = NULL;
  cl_device_id*   device_ids = NULL;

  // Check how many OpenCL platforms are available
  error = clGetPlatformIDs(0, NULL, &platform_count);
  if(0 == platform_count) {
    printf("No OpenCL platforms found\n");
    return 1;
  }


  // Obtain list of OpenCL platforms
  platform_ids = malloc(sizeof(cl_platform_id) * platform_count);
  error = clGetPlatformIDs(platform_count, platform_ids, NULL);
  if(error != CL_SUCCESS) {
    printf("Unable to obtain list of OpenCL platforms\n");
    return 1;
  }


  // Print list of available platforms
  for(cl_uint i = 0; i < platform_count; ++i) {
    error = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, PLATFORM_NAME_LEN, &platform_name, NULL);
    if(error != CL_SUCCESS) {
      printf("Unable to obtain name of OpenCL platform %u\n", i);
      return 1;
    }

    error = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, PLATFORM_VENDOR_LEN, &platform_vendor, NULL);
    if(error != CL_SUCCESS) {
      printf("Unable to obtain vendor of OpenCL platform %u\n", i);
      return 1;
    }

    printf("* OpenCL Platform %u: name = %s, vendor = %s\n", i, platform_name, platform_vendor);

    // Get device count for this platform
    error = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);
    if(error != CL_SUCCESS) {
      printf("Unable to devices of OpenCL platform %u\n", i);
      return 1;
    }

    // Obtain list of devices for this platform
    device_ids = malloc(sizeof(cl_device_id) * device_count);
    error = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, device_count, device_ids, NULL);
    if(error != CL_SUCCESS) {
      printf("Unable to obtain list of devices for OpenCL platform %u\n", i);
      return 1;
    }

    // Iterate over devices
    for(cl_uint j = 0; j < device_count; ++j) {
      error = clGetDeviceInfo(device_ids[j], CL_DEVICE_NAME, DEVICE_NAME_LEN, &device_name, NULL);
      if(error != CL_SUCCESS) {
        printf("Unable to obtain name of OpenCL device %u for platform %u\n", j, i);
        return 1;
      }

      error = clGetDeviceInfo(device_ids[j], CL_DEVICE_VENDOR, DEVICE_VENDOR_LEN, &device_vendor, NULL);
      if(error != CL_SUCCESS) {
        printf("Unable to obtain vendor of OpenCL device %u for platform %u\n", j, i);
        return 1;
      }

      error = clGetDeviceInfo(device_ids[j], CL_DEVICE_VERSION, DEVICE_VERSION_LEN, &device_version, NULL);
      if(error != CL_SUCCESS) {
        printf("Unable to obtain version of OpenCL device %u for platform %u\n", j, i);
        return 1;
      }

      error = clGetDeviceInfo(device_ids[j], CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
      if(error != CL_SUCCESS) {
        printf("Unable to obtain type of OpenCL device %u for platform %u\n", j, i);
        return 1;
      }

      switch(device_type) {
        case CL_DEVICE_TYPE_CPU:
          printf("  - Device %u (CPU): name = %s, vendor = %s, version = %s\n", j, device_name, device_vendor, device_version);
          break;

        case CL_DEVICE_TYPE_GPU:
          printf("  - Device %u (GPU): name = %s, vendor = %s, version = %s\n", j, device_name, device_vendor, device_version);
          break;

        case CL_DEVICE_TYPE_ACCELERATOR:
          printf("  - Device %u (ACCELERATOR): name = %s, vendor = %s, version = %s\n", j, device_name, device_vendor, device_version);
          break;
      }

    }
  }


  return 0;
}

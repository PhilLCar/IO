#ifndef EXPORT_H
#define EXPORT_H

#ifdef WIN
#define PUBLIC __declspec(dllexport)
#else
#define PUBLIC __attribute__((visibility("default")))
#endif

#endif
#ifndef EXPORT_H
#define EXPORT_H

#ifdef WIN
#define PUBLIC __declspec(dllexport)
#ifdef __INTELLISENSE__
#define __VA_OPT__(X)
#endif
#else
#define PUBLIC __attribute__((visibility("default")))
#endif

#endif
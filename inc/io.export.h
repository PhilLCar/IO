#ifndef IO_EXPORT_H
#define IO_EXPORT_H


#ifdef WIN
#ifdef IO_EXPORTS
#define IO_EXPORT __declspec(dllexport)
#else
#define IO_EXPORT __declspec(dllimport)
#endif
#ifdef __INTELLISENSE__
#define __VA_OPT__(X)
#endif
#else
#define IO_EXPORT __attribute__((visibility("default")))
#endif
#endif
#pragma once

extern "C" __declspec(dllexport) int Add(int &a,int &b); 

extern "C" __declspec(dllexport) int Max(const int &a, const int &b);
extern "C" __declspec(dllexport) int Get(const int &a);
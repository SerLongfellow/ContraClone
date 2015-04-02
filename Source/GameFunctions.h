#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

const float fPRV = 120.0f;

inline float PW2RW(float x){return x*fPRV;}

inline float RW2PW(float x){return x/fPRV;}

inline float RW2PW(int x){return (float)x/fPRV;}

#endif
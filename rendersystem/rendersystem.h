/**
 * 
 * rendersystem.h
 * 
 * Rendersystem is an advanced rendering backend for xash3d. It's based off of xash3d's original ref_gl renderer, but with some pretty significant changes.
 * 
 * The rendersystem abstracts graphics API calls in such a way that you can write platform agnostic rendering code
 * quite easily. The goal of this is primarily to ease shader creation and make extending the graphics system much much easier. 
 * 
 */
#pragma once
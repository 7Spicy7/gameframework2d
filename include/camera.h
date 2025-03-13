#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_vector.h"
#include "gfc_shape.h"

void camera_set_bounds(GFC_Rect bounds);

void camera_enable_binding(Bool bindCamera);

GFC_Vector2D camera_get_position();

GFC_Vector2D camera_get_offset();

GFC_Vector2D camera_set_position(GFC_Vector2D position);

GFC_Vector2D camera_set_size(GFC_Vector2D size);

void camera_apply_bounds();

void camera_center_on(GFC_Vector2D target);

#endif
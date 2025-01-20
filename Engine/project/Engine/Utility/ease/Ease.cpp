#include "Ease.h"

float Ease::InBack(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * t * t * t - c1 * t * t;
}

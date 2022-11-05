#pragma once
#include <d3dx9.h>
	struct BoundingBox
	{
		BoundingBox()
		{
		const float INFINITY = FLT_MAX;
		_min.x = INFINITY;
		_min.y = INFINITY;
		_min.z = INFINITY;
		_max.x = INFINITY;
		_max.y = INFINITY;
		_max.z = INFINITY;
		}

		bool isPointInside(D3DXVECTOR3& p)
		{
			if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z && p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
				{
					return true;
				}
			else
			{
					return false;
			}
		}

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};
	struct BoundingSphere
	{
		BoundingSphere()
		{
			_radius = 0.0f;
		}

		D3DXVECTOR3 _center;
		float       _radius;
	};

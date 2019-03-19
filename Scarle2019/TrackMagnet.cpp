#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"
#include "GameStateData.h"
#include "ServiceLocator.h"


TrackMagnet::TrackMagnet(string _filename) : PhysModel(_filename)
{
	m_autoCalculateWolrd = false;
	Vector3 scale = Vector3::Zero;
	m_world.Decompose(scale, m_quatRot, m_pos);
}

/* Checks for collision between this object and the track. 'Sticks' the object to the track if at a reasonable angle and distance */
bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	Vector intersect;
	Matrix targetWorld = Matrix::Identity;
	bool shouldStick = track.DoesLineIntersect(m_world.Down()*(data.m_height*30), m_pos + (m_world.Up() * (data.m_height/2)), intersect, tri, m_maxAngle);
	float modifiedMaxRotation = m_maxRotation;
	if (shouldStick)
	{
		Vector adjustVel = m_vel;
		// If velocity is opposite to direction, then the kart is reversing
		if ((m_vel + m_world.Forward()).Length() < m_vel.Length())
		{
			adjustVel *= -1;
		}
		// If the position of the kart is within the snapping area
		float dist = Vector::Distance(m_pos, intersect);
		if (dist > m_minSnapDist && dist < m_maxSnapDist)
		{
			// Turn gravity off when within the snapping zone, this smooths out movment
			m_gravVel = Vector3::Zero;
			m_gravDirection = Vector3::Zero;
			Vector3 moveVector = intersect - m_pos;
			if (moveVector.Length() > m_maxSnapSnep* Locator::getGSD()->m_dt)
			{
				moveVector.Normalize();
				moveVector *= m_maxSnapSnep* Locator::getGSD()->m_dt;
			}
			SetPos(m_pos + moveVector);
		}
		else if (dist > m_maxSnapDist)
		{
			modifiedMaxRotation /= 10;
			m_gravDirection = m_world.Down() * gravityMultiplier;
		}

		// Calculate a new rotation using 2 points on the plane that is found
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(m_world.Down() * (data.m_height * 30), m_pos + adjustVel + m_world.Forward() + (m_world.Up() * (data.m_height / 2)), secondIntersect, tri2, m_maxAngle);
		targetWorld = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
		targetWorld = Matrix::CreateScale(m_scale) * targetWorld;

		if (dist > m_minSnapDist && dist < m_maxSnapDist)
		{
			// Map the veclocity onto the track so the kart doesn't fly off or decellerate
			MapVectorOntoTri(m_vel, m_pos, targetWorld.Down(), tri);
		}
	}
	else
	{
		modifiedMaxRotation /= 10;
		Vector forward = m_world.Forward();
		forward.y = 0;
		targetWorld = m_world.CreateWorld(m_pos, forward, Vector::Up);
		targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
		m_gravDirection = m_world.Down() * gravityMultiplier;
	}
	// Update the position and rotation by breaking apart m_world
	Vector3 scale = Vector3::Zero;
	Quaternion rot = Quaternion::Identity;
	targetWorld.Decompose(scale, rot, m_pos);

	// Calculate the distance between the current and target rotation
	Quaternion inverseRot = Quaternion::Identity;
	m_quatRot.Inverse(inverseRot);
	float lerpDelta = (modifiedMaxRotation * Locator::getGSD()->m_dt) / (inverseRot*rot).Length();
	if (lerpDelta > 1)
	{
		lerpDelta = 1;
	}
	
	// Lerp the rotation by the calculated amount
	
	m_quatRot = Quaternion::Slerp(m_quatRot, rot, lerpDelta);

	// Rebuild m_world
	m_rot = Matrix::CreateFromQuaternion(m_quatRot);
	m_world = Matrix::CreateScale(m_scale) * m_rot * Matrix::CreateTranslation(m_pos);

	return shouldStick;
}

void TrackMagnet::ResolveWallCollisions(Track& walls)
{
	Vector leftSide = data.m_globalBackTopLeft - data.m_globalFrontTopLeft;
	Vector rightSide = data.m_globalBackTopRight - data.m_globalFrontTopRight;
	Vector frontSide = data.m_globalFrontTopRight - data.m_globalFrontTopLeft;
	Vector backSide = data.m_globalBackTopRight - data.m_globalBackTopLeft;

	Vector intersect = Vector::Zero;
	MeshTri* wallTri = nullptr;

	if (walls.DoesLineIntersect(leftSide, data.m_globalFrontTopLeft, intersect, wallTri, 5) ||
		walls.DoesLineIntersect(rightSide, data.m_globalFrontTopRight, intersect, wallTri, 5) ||
		walls.DoesLineIntersect(frontSide, data.m_globalFrontTopLeft, intersect, wallTri, 5) ||
		walls.DoesLineIntersect(backSide, data.m_globalBackTopLeft, intersect, wallTri, 5))
	{
		//std::cout << std::to_string(wallTri->m_pointA.x - m_pos.x) << std::endl;
		/*offset = (wallTri->m_pointA.x - m_pos.x) * 2;
		std::cout << std::to_string(offset) << std::endl;*/

		// Check if the velocity and this wall are not already diverging
		if ((wallTri->m_plane.Normal() + m_vel).Length() < m_vel.Length())
		{
			Vector prevVel = m_vel;
			Vector prevVelNorm = m_vel;
			prevVelNorm.Normalize();
			m_vel = Vector::Reflect(m_vel, wallTri->m_plane.Normal());

			// Map the end point of the vector back onto the track plane
			MapVectorOntoTri(m_vel, m_pos, m_world.Down(), tri);

			Vector velNorm = m_vel;
			velNorm.Normalize();

			// Return the velocity back to its previous magnitude
			m_vel = velNorm * prevVel.Length();

			if (m_dampenWallReflect)
			{
				float dist = Vector::Distance(velNorm, prevVelNorm);
				m_vel *= 1 - (dist / 2.4f);
			}
		}
	}

}

void TrackMagnet::MapVectorOntoTri(Vector& _vect, Vector& _startPos, Vector& _down, MeshTri * _tri)
{
	Vector endPoint = _startPos + _vect;
	Vector mappedToPlane = endPoint;
	MeshTri* tri2 = nullptr;
	_tri->DoesLineIntersect(_down, endPoint, mappedToPlane, tri2, 15);
	_vect = mappedToPlane - m_pos;
}
#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"
#include "GameStateData.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{
	m_autoCalculateWolrd = false;
	Vector3 scale = Vector3::Zero;
	m_world.Decompose(scale, m_quatRot, m_pos);
}

/* Checks for collision between this object and the track. 'Sticks' the object to the track if at a reasonable angle and distance */
bool TrackMagnet::ShouldStickToTrack(Track& track, GameStateData* _GSD)
{
	Vector intersect;
	Matrix targetWorld = Matrix::Identity;
	bool shouldStick = track.DoesLineIntersect(m_world.Down()*(m_height*30), m_pos + (m_world.Up() * m_height*2), intersect, tri, m_maxAngle);
	float modifiedMaxRotation = m_maxRotation;
	if (shouldStick)
	{
		Vector adjustVel = m_vel;
		// If velocity is opposite to direction, then the kart is reversing
		if ((m_vel + m_world.Forward()).Length() < m_vel.Length())
		{
			adjustVel *= -1;
		}
		// TODO: This offset is hard coded, change if the kart ever has bounds?
		Vector offset = m_world.Up() / 4;
		// If the position of the kart is within the snapping area
		float dist = Vector::Distance(m_pos + offset, intersect);
		if (dist > m_minSnapDist && dist < m_maxSnapDist)
		{
			// Turn gravity off when within the snapping zone, this smooths out movment
			m_gravVel = Vector3::Zero;
			m_gravDirection = Vector3::Zero;
			Vector3 moveVector = (intersect + offset) - m_pos;
			if (moveVector.Length() > m_maxSnapSnep* _GSD->m_dt)
			{
				moveVector.Normalize();
				moveVector *= m_maxSnapSnep* _GSD->m_dt;
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
		tri->DoesLineIntersect(m_world.Down() * (m_height * 30), m_pos + adjustVel + m_world.Forward() + (m_world.Up() * (m_height * 2)), secondIntersect, tri2, m_maxAngle);
		targetWorld = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
		targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
	}
	else
	{
		targetWorld = m_world.CreateWorld(m_pos, m_world.Forward(), Vector::Up);
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
	float lerpDelta = (modifiedMaxRotation * _GSD->m_dt) / (inverseRot*rot).Length();
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
	Vector leftSide = m_globalBackLeft - m_globalFrontLeft;
	Vector rightSide = m_globalBackRight - m_globalFrontRight;
	Vector frontSide = m_globalFrontRight - m_globalFrontLeft;
	Vector backSide = m_globalBackRight - m_globalBackLeft;

	Vector intersect = Vector::Zero;
	MeshTri* wallTri = nullptr;

	if (walls.DoesLineIntersect(leftSide, m_globalFrontLeft, intersect, wallTri, 15) ||
		walls.DoesLineIntersect(rightSide, m_globalFrontRight, intersect, wallTri, 15) ||
		walls.DoesLineIntersect(frontSide, m_globalFrontLeft, intersect, wallTri, 15) ||
		walls.DoesLineIntersect(backSide, m_globalFrontLeft, intersect, wallTri, 15))
	{
		// Check if the velocity and this wall are not already diverging
		if ((wallTri->m_plane.Normal() + m_vel).Length() < m_vel.Length())
		{
			Vector prevVel = m_vel;
			prevVel.Normalize();
			m_vel = Vector::Reflect(m_vel, wallTri->m_plane.Normal());

			// Map the end point of the vector back onto the track plane

			Vector endPoint = m_pos + m_vel;
			Vector mappedToPlane = endPoint;
			MeshTri* tri2 = nullptr;
			tri->DoesLineIntersect(m_world.Down(), endPoint, mappedToPlane, tri2, 15);
			m_vel = mappedToPlane - m_pos;

			Vector velNorm = m_vel;
			velNorm.Normalize();
			float dist = Vector::Distance(velNorm, prevVel);
			m_vel *= 1 - (dist / 2.4f);
		}
	}

}
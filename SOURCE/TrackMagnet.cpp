#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"
#include "GameStateData.h"
#include "ServiceLocator.h"


TrackMagnet::TrackMagnet(std::string _filename) : PhysModel(_filename)
{
	m_autoCalculateWolrd = false;
	Vector3 scale = Vector3::Zero;
	m_world.Decompose(scale, m_quatRot, m_pos);
}

void TrackMagnet::Render()
{
	SDKMeshGO3D::Render();
}

/* Checks for collision between this object and the track. 'Sticks' the object to the track if at a reasonable angle and distance */
bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	track.SetValidCollision(true, true, true, false);

	Vector intersect;
	Vector mid_intersect;
	Matrix targetWorld = Matrix::Identity;
	bool shouldStick = false;
	// Check the previous triangle if there was one
	if (tri)
	{
		shouldStick = tri->DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)), intersect, tri, m_maxAngle, 0) ||
			track.DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)), intersect, tri, m_maxAngle, 0);
	}
	// Otherwise check down from the middle
	else
	{
		bool shouldStick = track.DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)), intersect, tri, m_maxAngle, 0);
	}

	mid_intersect = intersect;
	// If the middle found nothing, check 3 more points
	if (!shouldStick)
	{
		shouldStick = track.DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)) + (m_world.Forward()), intersect, tri, m_maxAngle,0) ||
			track.DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)) + (m_world.Left()), intersect, tri, m_maxAngle,0) ||
			track.DoesLineIntersect(m_world.Down()*(data.m_height * 30), m_pos + (m_world.Up() * (data.m_height / 2)) + (m_world.Right()), intersect, tri, m_maxAngle, 0);
	}

	float modifiedMaxRotation = m_maxRotation;
	// If track was found
	if (shouldStick)
	{
		m_timeOffTerrain = 0;
		if (m_useGroundTypes)
		{
			m_colType = tri->GetType();
			switch (tri->GetType())
			{
			case ON_TRACK:
			{
				m_drag = 0.7f;
				break;
			}
			case OFF_TRACK:
			{
				m_drag = 3;
				break;
			}
			case BOOST_PAD:
			{
				Vector vel = m_vel;
				vel.Normalize();
				vel *= 200 * Locator::getGSD()->m_dt;
				m_vel += vel;
				break;
			}
			default:
				break;
			}
		}

		Vector adjustVel = m_vel;
		// If velocity is opposite to direction, then the kart is reversing
		if ((m_vel + m_world.Forward()).Length() < m_vel.Length())
		{
			adjustVel *= -1;
		}
		m_onTrack = false;
		// If the position of the kart is within the snapping area
		float dist = Vector::Distance(m_pos, intersect);
		if (dist > m_minSnapDist && dist < m_maxSnapDist)
		{
			m_onTrack = true;
			// Turn gravity off when within the snapping zone, this smooths out movment
			m_gravVel = Vector3::Zero;
			m_gravDirection = Vector3::Zero;
			Vector3 moveVector = mid_intersect - m_pos;
			if (moveVector.Length() > m_maxSnapSnep* Locator::getGSD()->m_dt)
			{
				moveVector.Normalize();
				moveVector *= m_maxSnapSnep* Locator::getGSD()->m_dt;
			}
			SetPos(m_pos + moveVector);
		}
		else if (dist > m_maxSnapDist)
		{
			modifiedMaxRotation /= 5;
			m_gravDirection = m_world.Down() * gravityMultiplier;
		}

		// Calculate a new rotation using 2 points on the plane that is found
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(m_world.Down() * (data.m_height * 30), m_pos + adjustVel + m_world.Forward() + (m_world.Up() * (data.m_height / 2)), secondIntersect, tri2, m_maxAngle,0);
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
		m_colType = CollisionType::NO_TERRAIN;
		modifiedMaxRotation /= 5;

		// Wait for bit, then if no track is found start rotating to the world up
		m_timeOffTerrain += Locator::getGSD()->m_dt;
		if (m_timeOffTerrain > m_rotationDelay)
		{
			targetWorld = m_world.CreateWorld(m_pos, m_vel, Vector::Up);
			targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
		}
		else
		{
			targetWorld = m_world.CreateWorld(m_pos, m_vel, m_world.Up());
			targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
		}
		m_gravDirection = m_world.Down() * gravityMultiplier;
	}

	// Check if this new world would collide with walls
	Matrix tempWorld = m_world;
	m_world = targetWorld;
	if (ResolveWallCollisions(track))
	{
		//targetWorld = tempWorld;
		//m_world = tempWorld;
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

bool TrackMagnet::ResolveWallCollisions(Track& walls)
{
	/*Vector leftSide = (data.m_globalBackTopLeft - data.m_globalFrontTopLeft) + (m_world.Down() *1);
	Vector rightSide = (data.m_globalBackTopRight - data.m_globalFrontTopRight) + (m_world.Down() * 1);
	Vector frontSide = (data.m_globalFrontTopRight - data.m_globalFrontTopLeft) + (m_world.Down() * 1);
	Vector backSide = (data.m_globalBackTopRight - data.m_globalBackTopLeft) + (m_world.Down() * 1);*/

	Vector3 sideOffset = m_world.Up() * (data.m_height / 2);

	Vector leftSide = (data.m_globalBackBottomLeft + sideOffset) - (data.m_globalFrontBottomLeft + sideOffset);
	Vector rightSide = (data.m_globalBackBottomRight + sideOffset) - (data.m_globalFrontBottomRight + sideOffset);
	Vector frontSide = (data.m_globalFrontBottomRight + sideOffset) - (data.m_globalFrontBottomLeft + sideOffset);
	Vector backSide = (data.m_globalBackBottomRight + sideOffset) - (data.m_globalBackBottomLeft + sideOffset);

	Vector cornerOffset = m_world.Up() * data.m_height*2;

	Vector frontLeft = data.m_globalFrontBottomLeft - (data.m_globalFrontTopLeft + cornerOffset);
	Vector frontRight = data.m_globalFrontBottomRight - (data.m_globalFrontTopRight + cornerOffset);
	Vector backLeft = data.m_globalBackBottomLeft - (data.m_globalBackTopLeft + cornerOffset);
	Vector backRight = data.m_globalBackBottomRight - (data.m_globalBackTopRight + cornerOffset);

	Vector intersect = Vector::Zero;
	MeshTri* wallTri = nullptr;

	// First check collision against walls only without angle limits
	walls.SetValidCollision(true, true, true, true);
	bool hit_wall = (walls.DoesLineIntersect(leftSide, data.m_globalFrontBottomLeft + sideOffset, intersect, wallTri, 5,0) ||
		walls.DoesLineIntersect(rightSide, data.m_globalFrontBottomRight + sideOffset, intersect, wallTri, 5,0) ||
		walls.DoesLineIntersect(frontSide, data.m_globalFrontBottomLeft + sideOffset, intersect, wallTri, 5,0) ||
		walls.DoesLineIntersect(backSide, data.m_globalBackBottomLeft + sideOffset, intersect, wallTri, 5,0));

	// Then if no collision is found check against non-walls with verticle lines and an angle limit
	walls.SetValidCollision(false, false, false, true);
	if (hit_wall || walls.DoesLineIntersect(frontLeft, data.m_globalFrontTopLeft + cornerOffset, intersect, wallTri, 5,m_minAngle) ||
		walls.DoesLineIntersect(frontRight, data.m_globalFrontTopRight + cornerOffset, intersect, wallTri, 5, m_minAngle) ||
		walls.DoesLineIntersect(backLeft, data.m_globalBackTopLeft + cornerOffset, intersect, wallTri, 5, m_minAngle) ||
		walls.DoesLineIntersect(backRight, data.m_globalBackTopRight + cornerOffset, intersect, wallTri, 5, m_minAngle))
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
			return true;
		}
	}
	return false;
}

void TrackMagnet::MapVectorOntoTri(Vector& _vect, Vector& _startPos, Vector _down, MeshTri * _tri)
{
	Vector endPoint = _startPos + _vect;
	Vector mappedToPlane = endPoint;
	MeshTri* tri2 = nullptr;
	_tri->DoesLineIntersect(_down, endPoint, mappedToPlane, tri2, 15,0);
	_vect = mappedToPlane - m_pos;
}
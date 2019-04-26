#include "pch.h"
#include "GameStateData.h"
#include "AnimationController.h"

void AnimationController::Render()
{
	if (m_shouldRender)
	{
		for (AnimationModel* model : m_modelSet[m_currentSet])
		{
			model->getModel()->Render();
		}
	}
}

void AnimationController::AddModelSet(std::string _setName, std::vector<std::string> models)
{
	std::vector<AnimationModel*> new_set;
	// Loop throug all the models and find the matching model names
	for (std::string& str : models)
	{
		for (auto& model : m_additionalModels)
		{
			if (model->getName() == str)
			{
				new_set.push_back(model.get());
			}
		}
	}
	using pair = std::pair<std::string, std::vector<AnimationModel*>>;
	m_modelSet.insert(pair(_setName, new_set));
}


void AnimationController::AddModel(std::string _name, SDKMeshGO3D* _model, SimpleMath::Vector3 _offset)
{
	m_additionalModels.push_back(std::make_unique<AnimationModel>(_model, _name, _offset));
}

void AnimationController::AddModel(std::string _name, std::string _filepath, Vector3 _offset)
{
	m_additionalModels.push_back(std::make_unique<AnimationModel>(new SDKMeshGO3D(_filepath), _name, _offset));
}

void AnimationController::Load() 
{
	for (auto& model : m_additionalModels)
	{
		model->getModel()->Load();
	}
}

void AnimationController::Reset()
{
	for (auto& model : m_additionalModels)
	{
		model->getModel()->Reset();
	}
}

void AnimationController::Update(Matrix _parentWorld, Vector3 _rotOffsetOverride)
{
	if (!m_posAnimPoints.empty())
	{
		m_posTimeElapsed += Locator::getGSD()->m_dt;
		if (m_posTimeElapsed > m_timeBetweenPos)
		{
			m_posTimeElapsed = m_timeBetweenPos;
		}
		m_posOffset = Vector3::Lerp(m_prevPoint, m_posAnimPoints.front(), m_posTimeElapsed / m_timeBetweenPos);
		if (Vector3::Distance(m_posOffset, m_posAnimPoints.front()) < 0.1f)
		{
			m_posTimeElapsed = 0;
			m_posOffset = m_posAnimPoints.front();
			m_prevPoint = m_posAnimPoints.front();
			m_posAnimPoints.pop();
		}
	}

	if (!m_rotAnimPoints.empty())
	{
		m_rotTimeElapsed += Locator::getGSD()->m_dt;
		if (m_rotTimeElapsed > m_timeBetweenRot)
		{
			m_rotTimeElapsed = m_timeBetweenRot;
		}
		switch (m_rotAnimPoints.front())
		{
		case LEFT:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Left(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Left());
			break;
		}
		case BACKWARD:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Backward(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Backward());
			break;
		}
		case RIGHT:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Right(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Right());
			break;
		}
		case FORWARD:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Forward(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Forward());
			break;
		}
		case UP:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Up(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Up());
			break;
		}
		case DOWN:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Down(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Down());
			break;
		}
		}

		//DebugText::print(std::to_string(m_rotOffset.x) + ",  " + std::to_string(m_rotOffset.y) + ", " + std::to_string(m_rotOffset.z));

	}
	else
	{
		m_rotOffset = Vector3::Lerp(m_rotOffset, _rotOffsetOverride, m_rotAnimSpeed * Locator::getGSD()->m_dt);
	}

	/*SetWorld(_parentWorld);

	SetWorld(Matrix::CreateScale(m_scale) * Matrix::CreateWorld(m_pos, m_rotOffset, m_world.Up()));

	m_pos += m_posOffset;
	UpdateWorld();*/

	UpdateWorld(_parentWorld);

	for (AnimationModel* model : m_modelSet[m_currentSet])
	{
		Vector3 scale_original = model->getModel()->GetScale();
		model->getModel()->SetWorld(m_world);
		model->getModel()->AddPos(Vector3::Transform(model->getOffset(), m_rot));
		model->getModel()->SetScale(scale_original);
		model->getModel()->UpdateWorld();
	}
}

void AnimationController::UpdateWorld(Matrix& _newWorld)
{
	m_world = _newWorld;
	m_world.Decompose(m_scale, m_quatRot, m_pos);
	m_rot = Matrix::CreateFromQuaternion(m_quatRot);

	m_world = Matrix::CreateScale(m_scale) * Matrix::CreateWorld(m_pos, m_rotOffset, m_world.Up());
	m_world.Decompose(m_scale, m_quatRot, m_pos);
	m_rot = Matrix::CreateFromQuaternion(m_quatRot);
	m_pos += m_posOffset;

	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);
	m_world = scale * m_rot * trans;
}

void AnimationController::Jump(float _jumpHeight, float _duration)
{
	if (m_posAnimPoints.empty())
	{
		m_timeBetweenPos = _duration / 2;
		m_posTimeElapsed = 0;
		Vector3 m_prevPoint = Vector3::Zero;
		m_posAnimPoints.push(m_world.Up() * _jumpHeight);
		m_posAnimPoints.push(Vector3::Zero);
	}
}

void AnimationController::Spin(int _revolutions, float _duration)
{
	if (m_rotAnimPoints.empty())
	{
		m_timeBetweenRot = _duration / (4 * _revolutions);
		m_rotTimeElapsed = 0;
		direction m_prevDirection = FORWARD;
		for (int i = 0; i < _revolutions; i++)
		{
			m_rotAnimPoints.push(LEFT);
			m_rotAnimPoints.push(BACKWARD);
			m_rotAnimPoints.push(RIGHT);
			m_rotAnimPoints.push(FORWARD);
		}
	}
}

void AnimationController::Flip(int _revolutions, float _duration)
{
	if (m_rotAnimPoints.empty())
	{
		m_timeBetweenRot = _duration / (4 * _revolutions);
		m_rotTimeElapsed = 0;
		direction m_prevDirection = FORWARD;
		for (int i = 0; i < _revolutions; i++)
		{
			m_rotAnimPoints.push(DOWN);
			m_rotAnimPoints.push(BACKWARD);
			m_rotAnimPoints.push(UP);
			m_rotAnimPoints.push(FORWARD);
		}
	}
}

void AnimationController::PopRotationQueue(Vector3 _targetRot)
{
	if (Vector3::Distance(m_rotOffset, _targetRot) < 0.1f)
	{
		m_rotTimeElapsed = 0;
		m_rotOffset = _targetRot;
		m_prevDirection = m_rotAnimPoints.front();
		m_rotAnimPoints.pop();
	}
}

Vector3 AnimationController::GetRotationFromDirection(direction _prevDirection, Matrix _world)
{
	switch (_prevDirection)
	{
	case LEFT:
	{
		return _world.Left();
	}
	case BACKWARD:
	{
		return _world.Backward();
	}
	case RIGHT:
	{
		return _world.Right();
	}
	case FORWARD:
	{
		return _world.Forward();
	}
	case UP:
	{
		return _world.Up();
	}
	case DOWN:
	{
		return _world.Down();
	}
	}
}

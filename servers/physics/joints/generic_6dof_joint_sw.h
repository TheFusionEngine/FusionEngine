#ifndef GENERIC_6DOF_JOINT_SW_H
#define GENERIC_6DOF_JOINT_SW_H

#include "servers/physics/joints_sw.h"
#include "servers/physics/joints/jacobian_entry_sw.h"


/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


/*
2007-09-09
Generic6DOFJointSW Refactored by Francisco Le?n
email: projectileman@yahoo.com
http://gimpact.sf.net
*/


//! Rotation Limit structure for generic joints
class G6DOFRotationalLimitMotorSW {
public:
    //! limit_parameters
    //!@{
    real_t m_loLimit;//!< joint limit
    real_t m_hiLimit;//!< joint limit
    real_t m_targetVelocity;//!< target motor velocity
    real_t m_maxMotorForce;//!< max force on motor
    real_t m_maxLimitForce;//!< max force on limit
    real_t m_damping;//!< Damping.
    real_t m_limitSoftness;//! Relaxation factor
    real_t m_ERP;//!< Error tolerance factor when joint is at limit
    real_t m_bounce;//!< restitution factor
    bool m_enableMotor;
    bool m_enableLimit;

    //!@}

    //! temp_variables
    //!@{
    real_t m_currentLimitError;//!  How much is violated this limit
    int m_currentLimit;//!< 0=free, 1=at lo limit, 2=at hi limit
    real_t m_accumulatedImpulse;
    //!@}

    G6DOFRotationalLimitMotorSW()
    {
	m_accumulatedImpulse = 0.f;
	m_targetVelocity = 0;
	m_maxMotorForce = 0.1f;
	m_maxLimitForce = 300.0f;
	m_loLimit = -1e30;
	m_hiLimit = 1e30;
	m_ERP = 0.5f;
	m_bounce = 0.0f;
	m_damping = 1.0f;
	m_limitSoftness = 0.5f;
	m_currentLimit = 0;
	m_currentLimitError = 0;
	m_enableMotor = false;
	m_enableLimit=false;
    }

    G6DOFRotationalLimitMotorSW(const G6DOFRotationalLimitMotorSW & limot)
    {
	m_targetVelocity = limot.m_targetVelocity;
	m_maxMotorForce = limot.m_maxMotorForce;
	m_limitSoftness = limot.m_limitSoftness;
	m_loLimit = limot.m_loLimit;
	m_hiLimit = limot.m_hiLimit;
	m_ERP = limot.m_ERP;
	m_bounce = limot.m_bounce;
	m_currentLimit = limot.m_currentLimit;
	m_currentLimitError = limot.m_currentLimitError;
	m_enableMotor = limot.m_enableMotor;
    }



	//! Is limited
    bool isLimited()
    {
	if(m_loLimit>=m_hiLimit) return false;
	return true;
    }

	//! Need apply correction
    bool needApplyTorques()
    {
	if(m_currentLimit == 0 && m_enableMotor == false) return false;
	return true;
    }

	//! calculates  error
	/*!
	calculates m_currentLimit and m_currentLimitError.
	*/
	int testLimitValue(real_t test_value);

	//! apply the correction impulses for two bodies
    real_t solveAngularLimits(real_t timeStep,Vector3& axis, real_t jacDiagABInv,BodySW * body0, BodySW * body1);


};



class G6DOFTranslationalLimitMotorSW
{
public:
	Vector3 m_lowerLimit;//!< the constraint lower limits
    Vector3 m_upperLimit;//!< the constraint upper limits
    Vector3 m_accumulatedImpulse;
    //! Linear_Limit_parameters
    //!@{
    Vector3	m_limitSoftness;//!< Softness for linear limit
    Vector3	m_damping;//!< Damping for linear limit
    Vector3	m_restitution;//! Bounce parameter for linear limit
    //!@}
    bool enable_limit[3];

    G6DOFTranslationalLimitMotorSW()
    {
	m_lowerLimit=Vector3(0.f,0.f,0.f);
	m_upperLimit=Vector3(0.f,0.f,0.f);
	m_accumulatedImpulse=Vector3(0.f,0.f,0.f);

	m_limitSoftness = Vector3(1,1,1)*0.7f;
	m_damping = Vector3(1,1,1)*real_t(1.0f);
	m_restitution = Vector3(1,1,1)*real_t(0.5f);

	enable_limit[0]=true;
	enable_limit[1]=true;
	enable_limit[2]=true;
    }

    G6DOFTranslationalLimitMotorSW(const G6DOFTranslationalLimitMotorSW & other )
    {
	m_lowerLimit = other.m_lowerLimit;
	m_upperLimit = other.m_upperLimit;
	m_accumulatedImpulse = other.m_accumulatedImpulse;

	m_limitSoftness = other.m_limitSoftness ;
	m_damping = other.m_damping;
	m_restitution = other.m_restitution;
    }

    //! Test limit
	/*!
    - free means upper < lower,
    - locked means upper == lower
    - limited means upper > lower
    - limitIndex: first 3 are linear, next 3 are angular
    */
    inline bool	isLimited(int limitIndex)
    {
       return (m_upperLimit[limitIndex] >= m_lowerLimit[limitIndex]);
    }


    real_t solveLinearAxis(
	real_t timeStep,
	real_t jacDiagABInv,
	BodySW* body1,const Vector3 &pointInA,
	BodySW* body2,const Vector3 &pointInB,
	int limit_index,
	const Vector3 & axis_normal_on_a,
		const Vector3 & anchorPos);


};


class Generic6DOFJointSW : public JointSW
{
protected:


	union {
		struct {
			BodySW *A;
			BodySW *B;
		};

		BodySW *_arr[2];
	};

	//! relative_frames
    //!@{
	Transform3D	m_frameInA;//!< the constraint space w.r.t body A
    Transform3D	m_frameInB;//!< the constraint space w.r.t body B
    //!@}

    //! Jacobians
    //!@{
    JacobianEntrySW	m_jacLinear[3];//!< 3 orthogonal linear constraints
    JacobianEntrySW	m_jacAng[3];//!< 3 orthogonal angular constraints
    //!@}

	//! Linear_Limit_parameters
    //!@{
    G6DOFTranslationalLimitMotorSW m_linearLimits;
    //!@}


    //! hinge_parameters
    //!@{
    G6DOFRotationalLimitMotorSW m_angularLimits[3];
	//!@}


protected:
    //! temporal variables
    //!@{
    real_t m_timeStep;
    Transform3D m_calculatedTransformA;
    Transform3D m_calculatedTransformB;
    Vector3 m_calculatedAxisAngleDiff;
    Vector3 m_calculatedAxis[3];

	Vector3 m_AnchorPos; // point betwen pivots of bodies A and B to solve linear axes

    bool	m_useLinearReferenceFrameA;

    //!@}

    Generic6DOFJointSW&	operator=(Generic6DOFJointSW&	other)
    {
	ERR_PRINT("pito");
	(void) other;
	return *this;
    }



    void buildLinearJacobian(
	JacobianEntrySW & jacLinear,const Vector3 & normalWorld,
	const Vector3 & pivotAInW,const Vector3 & pivotBInW);

    void buildAngularJacobian(JacobianEntrySW & jacAngular,const Vector3 & jointAxisW);


	//! calcs the euler angles between the two bodies.
    void calculateAngleInfo();



public:
    Generic6DOFJointSW(BodySW* rbA, BodySW* rbB, const Transform3D& frameInA, const Transform3D& frameInB ,bool useLinearReferenceFrameA);

    virtual PhysicsServer::JointType get_type() const { return PhysicsServer::JOINT_6DOF; }

    virtual bool setup(float p_step);
    virtual void solve(float p_step);


	//! Calcs global transform of the offsets
	/*!
	Calcs the global transform for the joint offset for body A an B, and also calcs the agle differences between the bodies.
	\sa Generic6DOFJointSW.getCalculatedTransformA , Generic6DOFJointSW.getCalculatedTransformB, Generic6DOFJointSW.calculateAngleInfo
	*/
    void calculateTransforms();

	//! Gets the global transform of the offset for body A
    /*!
    \sa Generic6DOFJointSW.getFrameOffsetA, Generic6DOFJointSW.getFrameOffsetB, Generic6DOFJointSW.calculateAngleInfo.
    */
    const Transform3D & getCalculatedTransformA() const
    {
	return m_calculatedTransformA;
    }

    //! Gets the global transform of the offset for body B
    /*!
    \sa Generic6DOFJointSW.getFrameOffsetA, Generic6DOFJointSW.getFrameOffsetB, Generic6DOFJointSW.calculateAngleInfo.
    */
    const Transform3D & getCalculatedTransformB() const
    {
	return m_calculatedTransformB;
    }

    const Transform3D & getFrameOffsetA() const
    {
	return m_frameInA;
    }

    const Transform3D & getFrameOffsetB() const
    {
	return m_frameInB;
    }


    Transform3D & getFrameOffsetA()
    {
	return m_frameInA;
    }

    Transform3D & getFrameOffsetB()
    {
	return m_frameInB;
    }


	//! performs Jacobian calculation, and also calculates angle differences and axis


    void	updateRHS(real_t	timeStep);

	//! Get the rotation axis in global coordinates
	/*!
	\pre Generic6DOFJointSW.buildJacobian must be called previously.
	*/
    Vector3 getAxis(int axis_index) const;

    //! Get the relative Euler angle
    /*!
	\pre Generic6DOFJointSW.buildJacobian must be called previously.
	*/
    real_t getAngle(int axis_index) const;

	//! Test angular limit.
	/*!
	Calculates angular correction and returns true if limit needs to be corrected.
	\pre Generic6DOFJointSW.buildJacobian must be called previously.
	*/
    bool testAngularLimitMotor(int axis_index);

    void	setLinearLowerLimit(const Vector3& linearLower)
    {
	m_linearLimits.m_lowerLimit = linearLower;
    }

    void	setLinearUpperLimit(const Vector3& linearUpper)
    {
	m_linearLimits.m_upperLimit = linearUpper;
    }

    void	setAngularLowerLimit(const Vector3& angularLower)
    {
	m_angularLimits[0].m_loLimit = angularLower.x;
	m_angularLimits[1].m_loLimit = angularLower.y;
	m_angularLimits[2].m_loLimit = angularLower.z;
    }

    void	setAngularUpperLimit(const Vector3& angularUpper)
    {
	m_angularLimits[0].m_hiLimit = angularUpper.x;
	m_angularLimits[1].m_hiLimit = angularUpper.y;
	m_angularLimits[2].m_hiLimit = angularUpper.z;
    }

	//! Retrieves the angular limit informacion
    G6DOFRotationalLimitMotorSW * getRotationalLimitMotor(int index)
    {
	return &m_angularLimits[index];
    }

    //! Retrieves the  limit informacion
    G6DOFTranslationalLimitMotorSW * getTranslationalLimitMotor()
    {
	return &m_linearLimits;
    }

    //first 3 are linear, next 3 are angular
    void setLimit(int axis, real_t lo, real_t hi)
    {
	if(axis<3)
	{
		m_linearLimits.m_lowerLimit[axis] = lo;
		m_linearLimits.m_upperLimit[axis] = hi;
	}
	else
	{
		m_angularLimits[axis-3].m_loLimit = lo;
		m_angularLimits[axis-3].m_hiLimit = hi;
	}
    }

	//! Test limit
	/*!
    - free means upper < lower,
    - locked means upper == lower
    - limited means upper > lower
    - limitIndex: first 3 are linear, next 3 are angular
    */
    bool	isLimited(int limitIndex)
    {
	if(limitIndex<3)
	{
			return m_linearLimits.isLimited(limitIndex);

	}
	return m_angularLimits[limitIndex-3].isLimited();
    }

    const BodySW* getRigidBodyA() const
    {
	return A;
    }
    const BodySW* getRigidBodyB() const
    {
	return B;
    }

	virtual void calcAnchorPos(void); // overridable

    void set_param(Vector3::Axis p_axis,PhysicsServer::G6DOFJointAxisParam p_param, float p_value);
    float get_param(Vector3::Axis p_axis,PhysicsServer::G6DOFJointAxisParam p_param) const;

    void set_flag(Vector3::Axis p_axis,PhysicsServer::G6DOFJointAxisFlag p_flag, bool p_value);
    bool get_flag(Vector3::Axis p_axis,PhysicsServer::G6DOFJointAxisFlag p_flag) const;

};


#endif // GENERIC_6DOF_JOINT_SW_H

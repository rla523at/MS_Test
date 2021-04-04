#pragma once

#include "EquationOfState.h"

#include <math.h> //std::atan(1)

class InitialCondition
{
protected:
	const double pi_ = 4 * std::atan(1);

public:
	virtual ~InitialCondition(void) = default;

	virtual MathVector operator()(const MathVector& points) const = 0;

	virtual MathVector calculate_Exact_Solution(const double target_time, const MathVector& target_point) const;

protected:
	InitialCondition(void) = default;
};


#define INITIALCONDITION_FACTORY InitialConditionFactory::instance()
class InitialConditionFactory
{
private:
	InitialConditionFactory(void) = default;

public:
	InitialConditionFactory(const InitialConditionFactory& other) = delete;
	InitialConditionFactory(InitialConditionFactory&& other) = delete;

	static InitialConditionFactory& instance(void) {
		static InitialConditionFactory instance;
		return instance;
	}

	std::unique_ptr<InitialCondition> order(const std::string& initial_condition_name, const size_t dimension) const;
};


class SineAdvection2D : public InitialCondition
{
private:
	const double u_ = 1.0;
	const double v_ = 2.0;
	const double p_ = 1.0;

public:
	MathVector operator()(const MathVector& point) const override;

	MathVector calculate_Exact_Solution(const double target_time, const MathVector& target_point) const override;
};


class ModifiedSod2D : public InitialCondition
{
public:
	MathVector operator()(const MathVector& point) const override;
};


class Constant2D : public InitialCondition
{
public:
	MathVector operator()(const MathVector& point) const override {
		return EOS::convert_To_Conservative_2D({ 1,1,1,1 });
	}
};
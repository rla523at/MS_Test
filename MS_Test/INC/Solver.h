#pragma once

#include "Element.h"
#include "Profiler.h"
#include "TecPlot.h"


class SolverStop
{
protected:
	double stop_condition_constant_;

public:
	SolverStop(const double stop_condition_constant)
		: stop_condition_constant_(stop_condition_constant) {};

	virtual bool check(const size_t num_iteration, const double current_time, double& dt) const = 0;
};


class SolverStopByIteration : public SolverStop
{
public:
	SolverStopByIteration(const double stop_condition_constant)
		: SolverStop(stop_condition_constant) {};

	bool check(const size_t num_iteration, const double current_time, double& dt) const override {
		if (num_iteration >= this->stop_condition_constant_)
			return true;
		else
			return false;
	};
};


class SolverStopByTime : public SolverStop
{
public:
	SolverStopByTime(const double stop_condition_constant)
		: SolverStop(stop_condition_constant) {};

	bool check(const size_t num_iteration, const double current_time, double& dt) const override {
		if (current_time + dt >= stop_condition_constant_) {
			dt = stop_condition_constant_ - current_time;
			return true;
		}
		else
			return false;
	}
};


class SolverPostCondition
{
protected:
	size_t num_post_ = 1;

	double post_condition_constant_;

public:
	SolverPostCondition(const double post_condition_constant)
		: post_condition_constant_(post_condition_constant) {};

	virtual bool check(const size_t num_iteration, const double current_time, double& dt) = 0;
};


class SolverPostByIteration : public SolverPostCondition
{
public:
	SolverPostByIteration(const double post_condition_constant)
		: SolverPostCondition(post_condition_constant) {};

	bool check(const size_t num_iteration, const double current_time, double& dt) override {
		if (num_iteration >= this->post_condition_constant_ * this->num_post_) {
			++this->num_post_;
			return true;
		}
		else
			return false;
	};
};


class SolverPostByTime : public SolverPostCondition
{
public:
	SolverPostByTime(const double post_condition_constant)
		: SolverPostCondition(post_condition_constant) {};

	bool check(const size_t num_iteration, const double current_time, double& dt) override {
		const auto post_time = this->post_condition_constant_ * this->num_post_;
		if (current_time + dt >= post_time) {
			dt = post_time - current_time;
			++this->num_post_;
			return true;
		}
		else
			return false;
	}
};


class TimeScheme
{
public:
	virtual void update(ElementSet& element_set, const double dt) const = 0;

protected:
	std::unordered_map<size_t, RowMajorMatrix> calculate_Cell_Index_To_RHS(const ElementSet& element_set) const;
};


class TVDRK : public TimeScheme
{
public:
	void update(ElementSet& element_set, const double dt) const override;
};


class Solver
{
private:
	std::unique_ptr<PhysicalFlux> physical_flux_function_;

	std::unique_ptr<NumericalFlux> numerical_flux_function_;

	std::unique_ptr<TimeScheme> time_scheme_;

	std::unique_ptr<SolverPostCondition> post_condition_;

	std::unique_ptr<SolverStop> stop_condition_;

public:
	void solve(ElementSet& element_set, const SolverOption& solver_option);	

private:
	double calculate_dt(const std::vector<Cell>& cell_set) const;

	void set_Stop_Condition(const std::string& stop_condition_type, const double stop_condition_constant);

	void set_Post_Condition(const std::string& post_condition_type, const double post_condition_constant);

	void set_Time_Scheme(const std::string& time_scheme_name);

	void set_Physical_Flux(const std::string& governing_equation, const size_t dimension);

	void set_Numerical_Flux(const std::string& numerical_flux_scheme, const size_t dimension);
};



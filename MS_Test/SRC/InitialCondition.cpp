
#include "../INC/InitialCondition.h"

MathVector InitialCondition::calculate_Exact_Solution(const double target_time, const MathVector& target_position_vector) const{
	FATAL_ERROR("this initial condition doesn't support exact solution.");
	return  MathVector();
}

std::unique_ptr<InitialCondition> InitialConditionFactory::order(const std::string& initial_condition_name, const size_t dimension) const{
	const auto indent = initial_condition_name + Editor::to_String(dimension);
	if (StringEditor::is_same_without_Case_Sensitivity(indent, "SineAdvection2"))
		return std::make_unique<SineAdvection2D>();
	else if (StringEditor::is_same_without_Case_Sensitivity(indent, "ModifiedSod2"))
		return std::make_unique<ModifiedSod2D>();
	else if (StringEditor::is_same_without_Case_Sensitivity(indent, "Constant2"))
		return std::make_unique<Constant2D>();
	else{
		FATAL_ERROR(indent + " is not supported initial condition");
		return nullptr;
	}
}

MathVector SineAdvection2D::operator()(const MathVector& position_vector) const{
	const auto x0 = position_vector[0];
	const auto x1 = position_vector[1];

	const auto rho = 1.0 + 0.2 * std::sin(2.0 * this->pi_ * x0) * std::sin(2.0 * this->pi_ * x1);

	return EOS::convert_To_Conservative_2D({ rho, this->u_, this->v_, this->p_ });
}

MathVector SineAdvection2D::calculate_Exact_Solution(const double target_time, const MathVector& position_vector) const{
	const auto t = target_time;
	const auto x0 = position_vector[0];
	const auto x1 = position_vector[1];

	const auto rho = 1.0 + 0.2 * std::sin(2.0 * this->pi_ * (x0 - this->u_ * t)) * std::sin(2.0 * this->pi_ * (x1 - this->v_ * t));

	return EOS::convert_To_Conservative_2D({ rho, this->u_, this->v_, this->p_ });
}

MathVector ModifiedSod2D::operator()(const MathVector& position_vector) const{
	const auto x0 = position_vector[0];

	if (x0 <= 0.3){
		constexpr double rho = 1.0;
		constexpr double rhou = 0.75;
		constexpr double rhov = 0.0;
		constexpr double rhoE = 2.78125;

		return { rho, rhou, rhov, rhoE };
	}
	else{
		constexpr double rho = 0.125;
		constexpr double rhou = 0.0;
		constexpr double rhov = 0.0;
		constexpr double rhoE = 0.25;

		return { rho, rhou, rhov, rhoE };
	}	 
}

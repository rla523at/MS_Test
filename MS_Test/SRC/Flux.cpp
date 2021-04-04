#include "../INC/Flux.h"

RowMajorMatrix EulerFlux2D::operator()(const MathVector& solution) const{
	constexpr size_t num_equation = 4;
	constexpr size_t dimension = 2;
	const auto gamma = EOS::calculate_Gamma_With_Conservative(solution);

	const auto rho = solution[0];
	const auto rhou = solution[1];
	const auto rhov = solution[2];
	const auto rhoE = solution[3];

	const auto one_over_rho = 1.0 / rho;
	const auto u = rhou * one_over_rho;
	const auto v = rhov * one_over_rho;

	const auto rho_u2 = rhou * u;
	const auto rhouv = rhou * v;
	const auto rho_v2 = rhov * v;

	const auto p = (gamma - 1) * (rhoE - 0.5 * (rho_u2 + rho_v2));
			
	RowMajorMatrix flux(MatrixType::Full, num_equation, dimension);
	flux.at(0, 0) = rhou;				flux.at(0, 1) = rhov;
	flux.at(1, 0) = rho_u2 + p;			flux.at(1, 1) = rhouv;
	flux.at(2, 0) = rhouv;				flux.at(2, 1) = rho_v2 + p;
	flux.at(3, 0) = (rhoE + p) * u;		flux.at(3, 1) = (rhoE + p) * v;

	return flux;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MathVector LocalLaxFridrichFlux2D::operator()(const MathVector& solution_o, const MathVector& solution_n, const MathVector& normal_vector) const{
	constexpr size_t num_equation = 4;

	const auto& nx = normal_vector[0];
	const auto& ny = normal_vector[1];

	//
	const auto gamma_o = EOS::calculate_Gamma_With_Conservative(solution_o);
	const auto rho_o = solution_o[0];
	const auto rhou_o = solution_o[1];
	const auto rhov_o = solution_o[2];
	const auto rhoE_o = solution_o[3];

	const auto one_over_rho_o = 1.0 / rho_o;
	const auto u_o = rhou_o * one_over_rho_o;
	const auto v_o = rhov_o * one_over_rho_o;

	const auto p_o = (gamma_o - 1) * (rhoE_o - 0.5 * (rhou_o * u_o + rhov_o * v_o));

	const auto V_o = u_o * nx + v_o * ny;
	const auto a_o = std::sqrt(gamma_o * p_o * one_over_rho_o);


	//
	const auto gamma_n = EOS::calculate_Gamma_With_Conservative(solution_n);
	const auto rho_n = solution_n[0];
	const auto rhou_n = solution_n[1];
	const auto rhov_n = solution_n[2];
	const auto rhoE_n = solution_n[3];

	const auto one_over_rho_n = 1.0 / rho_n;
	const auto u_n = rhou_n * one_over_rho_n;
	const auto v_n = rhov_n * one_over_rho_n;

	const auto p_n = (gamma_n - 1) * (rhoE_n - 0.5 * (rhou_n * u_n + rhov_n * v_n));

	const auto V_n = u_n * nx + v_n * ny;
	const auto a_n = std::sqrt(gamma_n * p_n * one_over_rho_n);

	//
	const auto r_max = std::max(std::abs(V_o) + a_o, std::abs(V_n) + a_n) * 0.5;
	
	const auto p_o_p_n = p_o + p_n;

	// 1/2 x ( F(q+) + F(q-)) + r_max(q+ - q-)
	MathVector numerical_flux(num_equation);
	numerical_flux[0] = 0.5 * (rho_o * V_o + rho_n * V_n) + r_max * (rho_o - rho_n);
	numerical_flux[1] = 0.5 * (rhou_o * V_o + rhou_n * V_n + nx * p_o_p_n) + r_max * (rhou_o - rhou_n);
	numerical_flux[2] = 0.5 * (rhov_o * V_o + rhov_n * V_n + ny * p_o_p_n) + r_max * (rhov_o - rhov_n);
	numerical_flux[3] = 0.5 * ((rhoE_o + p_o) * V_o + (rhoE_n + p_n) * V_n) + r_max * (rhoE_o - rhoE_n);

	return numerical_flux;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const BoundaryFlux* BoundaryFluxFactory::order(const std::string& type_name, const NumericalFlux& numerical_flux_function){
	if (this->ware_house_.find(type_name) == this->ware_house_.end())
		ware_house_.emplace(type_name, produce(type_name, numerical_flux_function));

	return ware_house_.at(type_name).get();
}

std::unique_ptr<BoundaryFlux> BoundaryFluxFactory::produce(const std::string& type_name) const{
	if (StringEditor::is_same_without_Case_Sensitivity(type_name, "Slipwall2D"))
		return std::make_unique<SlipWall2D>();
	else if (StringEditor::is_same_without_Case_Sensitivity(type_name, "SupersonicOutlet2D"))
		return std::make_unique<SuperSonicOutlet>();
	else if (StringEditor::is_same_without_Case_Sensitivity(type_name, "SupersonicOutlet3D"))
		return std::make_unique<SuperSonicOutlet>();
	else{
		FATAL_ERROR(Editor::to_String(type_name) << " is not supproted boundary flux function");
		return nullptr;
	}
}

std::unique_ptr<BoundaryFlux> BoundaryFluxFactory::produce(const std::string& type_name, const NumericalFlux& numerical_flux_function) const{
	if (StringEditor::is_same_without_Case_Sensitivity(type_name, "Slipwall2D"))				
		return std::make_unique<SlipWall2D>(numerical_flux_function);
	else if (StringEditor::is_same_without_Case_Sensitivity(type_name, "SupersonicOutlet2D"))	
		return std::make_unique<SuperSonicOutlet>(numerical_flux_function);
	else if (StringEditor::is_same_without_Case_Sensitivity(type_name, "SupersonicOutlet3D"))	
		return std::make_unique<SuperSonicOutlet>(numerical_flux_function);
	else {
		FATAL_ERROR(Editor::to_String(type_name) << " is not supproted boundary flux function");
		return nullptr;
	}
}


MathVector SlipWall2D::operator()(const MathVector& owner_cell_solution, const MathVector& normal_vector) const{
	const double p = EOS::calculate_Pressure(owner_cell_solution);

	const size_t num_equation = 4;
	MathVector value(num_equation);

	value[0] = 0.0;
	value[1] = p * normal_vector[0];
	value[2] = p * normal_vector[1];
	value[3] = 0.0;

	return value;
}

MathVector SuperSonicOutlet::operator()(const MathVector& owner_cell_solution, const MathVector& normal_vector) const{
	return (*this->numerical_flux_function_)(owner_cell_solution, owner_cell_solution, normal_vector);
}
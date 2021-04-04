#include "../INC/Solver.h"

std::unordered_map<size_t, RowMajorMatrix> TimeScheme::calculate_Cell_Index_To_RHS(const ElementSet& element_set) const {
	const auto& [cell_set,boundary_face_set, inner_face_set, periodic_face_set] = element_set;
	const auto num_cell = cell_set.size();
		
	std::unordered_map<size_t, RowMajorMatrix> cell_index_to_rhs;
	cell_index_to_rhs.reserve(num_cell);

	for (const auto& cell : cell_set)
		cell_index_to_rhs.emplace(cell.index(), cell.calculate_Conservation_Law_Integral());
	for (const auto& boundary_face : boundary_face_set)
		boundary_face.calculate_Conservation_Law_Integral(cell_index_to_rhs);
	for (const auto& inner_face : inner_face_set)
		inner_face.calculate_Conservation_Law_Integral(cell_index_to_rhs);
	for (const auto& periodic_face : periodic_face_set)
		periodic_face.calculate_Conservation_Law_Integral(cell_index_to_rhs);

	return cell_index_to_rhs;
}

void TVDRK::update(ElementSet& element_set, const double dt) const {
	auto& cell_set = element_set.cell_set;
	const auto num_cell = cell_set.size();

	std::unordered_map<size_t, RowMajorMatrix> cell_index_to_original_coefficient;
	std::unordered_map<size_t, const RowMajorMatrix&> cell_index_to_current_coefficient;
	cell_index_to_original_coefficient.reserve(num_cell);
	cell_index_to_current_coefficient.reserve(num_cell);

	for (const auto& cell : cell_set) {
		const auto index = cell.index();
		const auto& coefficient = cell.coefficient();

		cell_index_to_original_coefficient.emplace(index, coefficient);
		cell_index_to_current_coefficient.emplace(index, coefficient);
	}

	//step 1
	const auto step1_cell_index_to_rhs = this->calculate_Cell_Index_To_RHS(element_set);
	for (auto& cell : cell_set) {
		const auto index = cell.index();
		const auto& original_coefficient = cell_index_to_original_coefficient.at(index);
		const auto& rhs_value = step1_cell_index_to_rhs.at(index);

		auto step1_coefficient = original_coefficient + rhs_value * dt;

		cell.update(std::move(step1_coefficient));
	}
	//limiter.limit(index_to_cell, face_set);

	//step 2
	const auto step2_cell_index_to_rhs = this->calculate_Cell_Index_To_RHS(element_set);
	for (auto& cell : cell_set)	{
		const auto index = cell.index();
		const auto& original_coefficient = cell_index_to_original_coefficient.at(index);
		const auto& current_coefficient = cell_index_to_current_coefficient.at(index);
		const auto& rhs_value = step2_cell_index_to_rhs.at(index);

		auto step2_coefficient = 0.25 * (3 * original_coefficient + current_coefficient + rhs_value * dt);

		cell.update(std::move(step2_coefficient));
	}
	//limiter.limit(index_to_cell, face_set);

	//step 3	
	const auto step3_cell_index_to_rhs = this->calculate_Cell_Index_To_RHS(element_set);
	for (auto& cell : cell_set) {
		const auto index = cell.index();
		const auto& original_coefficient = cell_index_to_original_coefficient.at(index);
		const auto& current_coefficient = cell_index_to_current_coefficient.at(index);
		const auto& rhs_value = step3_cell_index_to_rhs.at(index);

		auto new_coefficient = (1.0 / 3.0) * (original_coefficient + 2.0 * current_coefficient + 2.0 * rhs_value * dt);

		cell.update(std::move(new_coefficient));
	}
	//limiter.limit(index_to_cell, face_set);
}

void Solver::solve(ElementSet& element_set, const SolverOption& solver_option) {
	//set solver
	const auto& [dimension, governing_equation, numerical_flux_name, time_scheme_name, time_step_type_name, time_step_constant, stop_condition_type_name, stop_condition_constant, post_condition_type_name, post_condition_constant] = solver_option;

	this->set_Physical_Flux(governing_equation, dimension);
	this->set_Numerical_Flux(numerical_flux_name, dimension);
	this->set_Time_Scheme(time_scheme_name);
	this->set_Post_Condition(post_condition_type_name, post_condition_constant);
	this->set_Stop_Condition(stop_condition_type_name, stop_condition_constant);

	//initialize element interfaces
	auto& [cell_set, boundary_face_set, inner_face_set, periodic_face_set] = element_set;

	for (auto& cell : cell_set) {
		cell.initialize_Integral_Caclulator(*this->physical_flux_function_);
		cell.initialize_Time_Step_Calculator(time_step_type_name, time_step_constant);
	}
	for (auto& boundary_face : boundary_face_set) 
		boundary_face.initialize_Integral_Caclulator(*this->numerical_flux_function_);
	for (auto& inner_face : inner_face_set)
		inner_face.initialize_Integral_Caclulator(*this->numerical_flux_function_);
	for (auto& periodic_face : periodic_face_set)
		periodic_face.initialize_Integral_Caclulator(*this->numerical_flux_function_);

	std::cout << "\n\n============================================SOLUTION SOLVING============================================\n\n";

	double current_time = 0.0;
	size_t num_iteration = 1;

	RECORD_CONSUMED_MEMORY_AND_TIME;
	for (;;num_iteration++)	{
		RECORD_CONSUMED_TIME;
		auto dt = this->calculate_dt(element_set.cell_set);

		if (this->stop_condition_->check(num_iteration, current_time, dt)) {
			this->time_scheme_->update(element_set, dt);
			current_time += dt;

			TECPLOT.post_Solution_File(current_time, "final_solution");

			PROFILER_NOTICE << "Iteration : " << num_iteration << "    \tCurrent Time : " << current_time << "   \t";
			PRINT_CONSUMED_TIME;

			break;
		}

		if (this->post_condition_->check(num_iteration, current_time, dt)) {
			this->time_scheme_->update(element_set, dt);
			current_time += dt;

			TECPLOT.post_Solution_File(current_time);

			PROFILER_NOTICE << "Iteration : " << num_iteration << "    \tCurrent Time : " << current_time << "   \t";
			PRINT_CONSUMED_TIME;

			continue;
		}

		this->time_scheme_->update(element_set, dt);
		current_time += dt;
				
		PROFILER_NOTICE << "Iteration : " << num_iteration << "    \tCurrent Time : " << current_time << "   \t";
		PRINT_CONSUMED_TIME;
	}

	PROFILER_NOTICE << "\n\nSOLUTION SOLVING TOTAL\t";
	PRINT_CONSUMED_MEMORY_AND_TIME;;
	std::cout << "\n\n========================================================================================================\n\n";
}

double Solver::calculate_dt(const std::vector<Cell>& cell_set) const {
	const auto num_cell = cell_set.size();

	std::vector<double> time_step_set;
	time_step_set.reserve(num_cell);

	for (const auto& cell : cell_set) 
		time_step_set.emplace_back(cell.calculate_Time_Step());

	return *std::min_element(time_step_set.begin(), time_step_set.end());
}

void Solver::set_Stop_Condition(const std::string& stop_condition_type, const double stop_condition_constant) {
	if (StringEditor::is_same_without_Case_Sensitivity(stop_condition_type, "iteration"))
		this->stop_condition_ = std::make_unique<SolverStopByIteration>(stop_condition_constant);
	else if (StringEditor::is_same_without_Case_Sensitivity(stop_condition_type, "time"))
		this->stop_condition_ = std::make_unique<SolverStopByTime>(stop_condition_constant);
	else
		FATAL_TYPE_ERROR;
}

void Solver::set_Post_Condition(const std::string& post_condition_type, const double post_condition_constant) {
	if (StringEditor::is_same_without_Case_Sensitivity(post_condition_type, "iteration"))
		this->post_condition_ = std::make_unique<SolverPostByIteration>(post_condition_constant);
	else if (StringEditor::is_same_without_Case_Sensitivity(post_condition_type, "time"))
		this->post_condition_ = std::make_unique<SolverPostByTime>(post_condition_constant);
	else 
		FATAL_TYPE_ERROR;
}

void Solver::set_Time_Scheme(const std::string& time_scheme_name) {
	if (StringEditor::is_same_without_Case_Sensitivity(time_scheme_name, "TVDRK"))
		this->time_scheme_ = std::make_unique<TVDRK>();
	//else if (indent.isNoCaseSensitivitySame("SSPRK"))		return 	std::make_unique<SSPRK>();
	else
		FATAL_TYPE_ERROR;	
}

void Solver::set_Physical_Flux(const std::string& governing_equation, const size_t dimension) {
	std::string indent = governing_equation + Editor::to_String(dimension);
	if (StringEditor::is_same_without_Case_Sensitivity(indent, "Euler2"))
		this->physical_flux_function_ = std::make_unique<EulerFlux2D>();
	else
		FATAL_TYPE_ERROR;
}

void Solver::set_Numerical_Flux(const std::string& numerical_flux_scheme, const size_t dimension) {
	std::string indent = numerical_flux_scheme + Editor::to_String(dimension);
	if (StringEditor::is_same_without_Case_Sensitivity(indent, "LLF2"))
		this->numerical_flux_function_ = std::make_unique<LocalLaxFridrichFlux2D>();
	else 
		FATAL_TYPE_ERROR;	
}



#pragma once

#include "MathVector.h"

class EOS
{
public:
	static double calculate_Gamma_With_Conservative(const MathVector& solution) {
		return 1.4;
	}

	static double calculate_Gamma_With_Primitive(const MathVector& primitive_solution) {
		return 1.4;
	}

	static MathVector calculate_Velocity_Vector(const MathVector& solution)	{
		switch (solution.size()){
		case 3:{
			const auto rho = solution[0];
			const auto rhou = solution[1];

			return { rhou / rho };
		}
		case 4:{
			const auto rho = solution[0];
			const auto rhou = solution[1];
			const auto rhov = solution[2];

			const auto one_over_rho = 1.0 / rho;

			return { rhou * one_over_rho, rhov * one_over_rho };
		}
		case 5:{
			const auto rho = solution[0];
			const auto rhou = solution[1];
			const auto rhov = solution[2];
			const auto rhow = solution[3];

			const auto one_over_rho = 1.0 / rho;

			return { rhou * one_over_rho, rhov * one_over_rho, rhow * one_over_rho };
		}
		default:
			FATAL_TYPE_ERROR;
			return MathVector();
		}
	}

	static double calculate_Pressure(const MathVector& solution) {
		const double gamma = EOS::calculate_Gamma_With_Conservative(solution);

		const auto rho = solution.front();
		const auto rhoE = solution.back();

		const auto velocity = EOS::calculate_Velocity_Vector(solution);

		return (gamma - 1) * (rhoE - 0.5 * rho * (Math::inner_product(velocity, velocity)));
	}

	static double calculate_Pressure(const MathVector& solution, const MathVector& velocity) {
		const double gamma = EOS::calculate_Gamma_With_Conservative(solution);

		const auto rho = solution.front();
		const auto rhoE = solution.back();

		return (gamma - 1) * (rhoE - 0.5 * rho * (Math::inner_product(velocity, velocity)));
	}

	static double calculate_Pressure(const MathVector& solution, const double gamma, const MathVector& velocity) {
		const auto rho = solution.front();
		const auto rhoE = solution.back();

		return (gamma - 1) * (rhoE - 0.5 * rho * (Math::inner_product(velocity, velocity)));
	}

	static double calculate_Acoustic_Speed(const MathVector& solution){
		const auto rho = solution.front();

		const auto gamma = EOS::calculate_Gamma_With_Conservative(solution);
		const auto velocity = EOS::calculate_Velocity_Vector(solution);
		const auto p = EOS::calculate_Pressure(solution, gamma, velocity);
		
		return std::sqrt(gamma * p / rho);
	}

	static double calculate_Acoustic_Speed(const MathVector& solution, const MathVector& velocity) {
		const auto rho = solution.front();
		
		const auto gamma = EOS::calculate_Gamma_With_Conservative(solution);
		const auto p = EOS::calculate_Pressure(solution, gamma, velocity);

		return std::sqrt(gamma * p / rho);

	}

	static MathVector convert_To_Conservative_2D(const MathVector& primitive_solution){
		const auto gamma = EOS::calculate_Gamma_With_Primitive(primitive_solution);

		const auto rho = primitive_solution[0];
		const auto u = primitive_solution[1];
		const auto v = primitive_solution[2];
		const auto p = primitive_solution[3];

		const auto rhou = rho * u;
		const auto rhov = rho * v;
		const auto rhoE = p / (gamma - 1) + 0.5 * (rhou * u + rhov * v);

		return { rho,rhou,rhov,rhoE };
	}

	static MathVector convert_To_Primitive_2D(const MathVector& conservative_solution){
		const auto velocity = EOS::calculate_Velocity_Vector(conservative_solution);

		const auto rho = conservative_solution[0];
		const auto u = velocity[0];
		const auto v = velocity[1];
		const auto p = EOS::calculate_Pressure(conservative_solution, velocity);

		return { rho,u,v,p };
	}
};
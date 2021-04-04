#pragma once

#include "EquationOfState.h"
#include "Matrix.h"

#include <memory>
#include <map>

class PhysicalFlux
{
public:
	virtual ~PhysicalFlux(void) = default;
	virtual RowMajorMatrix operator()(const MathVector& solution) const = 0;
};


class EulerFlux2D : public PhysicalFlux
{
public:
	RowMajorMatrix operator()(const MathVector& solution) const override;
};


class NumericalFlux
{
public:
	virtual MathVector operator()(const MathVector& owner_cell_solution, const MathVector& neighbor_cell_solution, const MathVector& normal_vector) const = 0;
};


class LocalLaxFridrichFlux2D : public NumericalFlux
{
public:
	MathVector operator()(const MathVector& owner_cell_solution, const MathVector& neighbor_cell_solution, const MathVector& normal_vector) const override;	
};


class BoundaryFlux
{
protected:
	const NumericalFlux* numerical_flux_function_ = nullptr;	

public:
	virtual MathVector operator()(const MathVector& owner_cell_solution, const MathVector& normal_vector) const = 0;

	void setNumericalFlux(const NumericalFlux& numerical_flux_function) { this->numerical_flux_function_ = &numerical_flux_function; };
};


#define BOUNDARY_FLUX_FACTORY BoundaryFluxFactory::instance()
class BoundaryFluxFactory
{
private:
	std::map<std::string, std::unique_ptr<BoundaryFlux>> ware_house_;

public:
	BoundaryFluxFactory(const BoundaryFluxFactory& other) = delete;
	BoundaryFluxFactory(BoundaryFluxFactory&& other) = delete;

	static BoundaryFluxFactory& instance(void) {
		static BoundaryFluxFactory instance;
		return instance;
	}

	const BoundaryFlux* order(const std::string& type_name, const NumericalFlux& numerical_flux_function);

private:
	BoundaryFluxFactory(void) = default;

	std::unique_ptr<BoundaryFlux> produce(const std::string& type_name) const;

	std::unique_ptr<BoundaryFlux> produce(const std::string& type_name, const NumericalFlux& numerical_flux_function) const;
};


class SlipWall2D : public BoundaryFlux
{
public:
	SlipWall2D(void) = default;

	SlipWall2D(const NumericalFlux& numerical_flux_function){
		this->numerical_flux_function_ = &numerical_flux_function;
	};

	MathVector operator()(const MathVector& owner_cell_solution, const MathVector& normal_vector) const override;
};


class SuperSonicOutlet : public BoundaryFlux
{
public:
	SuperSonicOutlet(void) = default;

	SuperSonicOutlet(const NumericalFlux & numerical_flux_function) {
		this->numerical_flux_function_ = &numerical_flux_function;
	};

	MathVector operator()(const MathVector& owner_cell_solution, const MathVector& normal_vector) const override;
};
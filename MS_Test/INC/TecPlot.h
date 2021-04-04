#pragma once

#include "Config.h"
#include "Element.h"

class TecPlotSolutionData
{
protected:
	std::vector<RowMajorMatrix> basis_post_point_set_;

public:
	virtual std::vector<std::vector<double>> build(const std::vector<const Cell*>& post_target, const size_t num_total_post_point) const = 0;

	virtual std::string variables(void) const = 0;
};


class TecPlot2DConservativeSolutionData : public TecPlotSolutionData
{
protected:
	std::vector<std::vector<double>> build(const std::vector<const Cell*>& post_target, const size_t num_total_post_point) const override;

	std::string variables(void) const override {
		return "\"rho\" \"rhou\" \"rhov\" \"rhoE\"";
	}
};


class TecPlot2DPrimitiveSolutionData : public TecPlotSolutionData
{
protected:
	std::vector<std::vector<double>> build(const std::vector<const Cell*>& post_target, const size_t num_total_post_point) const override;

	std::string variables(void) const override {
		return "\"rho\" \"u\" \"v\" \"P\"";
	}
};


#define TECPLOT TecPlot::instance()
class TecPlot
{
private:
	std::string post_file_name_;

	size_t post_order_ = 1;

	std::unique_ptr<TecPlotSolutionData> solution_data_builder_;

	std::vector<const Cell*> post_target_;

	size_t num_total_post_point_ = 0;

	size_t num_simplex_ = 0;

	mutable size_t num_post_ = 1;

	std::string grid_variables_;

	std::string zone_type_;

public:
	static TecPlot& instance(void) {
		static TecPlot instance;
		return instance;
	}

	void initialize(PostOption&& post_option_data, std::vector<Cell>& cell_set);

	void post_Solution_File(const double solution_time = 0.0, const std::string& title = "") const;

private:
	Text write_File_Header_Text(const std::string& file_title, const std::string& file_type, const std::string& variables) const;

	Text write_Zone_Header_Text(const std::string& zone_title, const std::string& zone_type, const size_t num_nodes, const size_t num_elements, const std::string& data_packing_type, const size_t strand_id, const double solution_time) const;

	Text write_Zone_Data_Text(const std::vector<std::vector<double>>& data_block_set) const;

};

































//
//
//
//
//
//
//
//
//
//
//class TecPlotWriter
//{
//public:
//
//	virtual ~TecPlotWriter(void) = default;
//
//	virtual std::vector<std::vector<double>> build_Cell_Index_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const size_t num_post_point) const;
//	
//	virtual std::vector<std::vector<double>> build_Grid_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const size_t num_post_point) const = 0;
//
//	virtual std::vector<std::vector<double>> build_Solution_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const std::unordered_map<size_t, Cell>& index_to_cell, const size_t num_post_point) const = 0;
//
//	std::string write_Cell_Index_File_Variable(void) const { return "Variables = \"cell index\""; };
//
//	std::string write_File_Title(const std::string& title) const;
//	
//	std::string write_File_Type(const std::string& type) const;
//		
//	virtual std::string write_Grid_File_Variable(void) const = 0;
//	
//	std::string write_Num_Zone_Node(const size_t num_node) const;
//
//	std::string write_Num_Zone_Element(const size_t num_element) const;
//
//	virtual std::string write_Solution_File_Variable(void) const = 0;
//
//	std::string write_Zone_Data_Packing_Type(void) const { return "DataPacking = Block"; };
//
//	std::string write_Zone_Title(const std::string& title) const;
//	
//	virtual std::string write_Zone_Type(void) const = 0;	
//};
//
//
//
//class TecPlotWriter2D : public TecPlotWriter
//{
//public:
//
//	std::vector<std::vector<double>> build_Grid_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const size_t num_post_point) const override;
//
//	std::string write_Grid_File_Variable(void) const override {  };	
//
//	std::string write_Zone_Type(void) const override { return "ZoneType = FETRIANGLE"; };
//};
//
//
//
//class ConservativeVariableWriter2D : public TecPlotWriter2D
//{
//public:
//
//	std::vector<std::vector<double>> build_Solution_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const std::unordered_map<size_t, Cell>& index_to_cell, const size_t num_post_point) const override;
//
//	std::string write_Solution_File_Variable(void) const override { };
//};
//
//
//class PrimitiveVariableWriter2D : public TecPlotWriter2D
//{
//public:
//	std::vector<std::vector<double>> build_Solution_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const std::unordered_map<size_t, Cell>& index_to_cell, const size_t num_post_point) const override;
//
//	std::string write_Solution_File_Variable(void) const override { return "Variables = \"rho\" \"u\" \"v\" \"p\""; };
//};
//
//
//
//class Post
//{
//private:
//
//	const std::string post_file_name_;
//		
//	const size_t post_order_;
//	
//	mutable size_t num_post_ = 0;
//
//	TecPlotWriter* writer_ = nullptr;
//		
//	std::map<FigureType, std::vector<Node>> figure_type_to_reference_post_point_set_;
//
//	std::map<FigureType, std::vector<std::vector<size_t>>> figure_type_to_reference_connectivity_;	
//
//	std::unordered_map<size_t, std::vector<Node>> cell_index_to_post_point_set_;
//
//	std::vector<std::vector<size_t>> connectivity_set_;	// connectivity : post point를 연결한 simplex들의 연결 정보
//
//public:
//
//	Post(const size_t dimension, const std::string& post_file_path, const size_t post_order, const std::string& post_variable_type);
//
//	~Post(void) { delete this->writer_; };
//
//	void preCalculate(std::unordered_map<size_t, Cell>& index_to_cell);
//
//	void write_Cell_Index_File(void) const;
//
//	void write(void) const;
//
//	void write_Solution_File(const std::unordered_map<size_t, Cell>& index_to_cell, const double current_time) const;
//	
//	//void writeModeAnalysisFile(const Geometry& geometry, const SolutionFunction& solution_function);
//
//private:
//
//
//
//	Text write_File_Header_Text(const std::string& title, const std::string& file_type_sentence, const std::string& variable_sentence) const;
//
//	Text write_Zone_Header_Text(const std::string& title, const size_t num_post_point, const size_t num_simplex) const;
//		
//	Text write_Zone_Data_Text(const std::vector<std::vector<double>>& data_block_set) const;
//
//	Text write_Connectivity_Text(const std::vector<std::vector<size_t>>& connectivity_set) const;
//
//	
//};
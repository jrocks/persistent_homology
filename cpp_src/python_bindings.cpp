#include "embedding.hpp"
#include "filtration.hpp"

#include "cell_complex.hpp"
#include "network_complex.hpp"
#include "corner_complex.hpp"
#include "cubical_complex.hpp"
#include "morse_complex.hpp"
#include "morse_smale_complex.hpp"


#include "search.hpp"
#include "persistent_homology.hpp"
#include "persistence_simplification.hpp"
// #include "persistence_landscape.hpp"
    
#ifdef ALPHA
    #include "alpha_complex.hpp"
#endif  
    
#ifdef OPTIMAL
    #include "optimal.hpp"
#endif


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include <pybind11/eigen.h>
    
namespace py = pybind11;

template <int DIM> void init_embedding_templates(py::module &m) {
    
    py::class_<Embedding<DIM> >(m, (std::string("Embedding")+std::to_string(DIM)+std::string("D")).c_str())
        .def_readonly("dim", &Embedding<DIM>::dim)
        .def_readonly("box_mat", &Embedding<DIM>::box_mat)
        .def_readonly("periodic", &Embedding<DIM>::periodic)
        .def(py::init<RXVec, RDMat, bool>())
        .def("get_vpos", &Embedding<DIM>::get_vpos)
        .def("get_pos", &Embedding<DIM>::get_pos);

        
}

template <int DIM> void init_network_templates(py::module &m) {
    
    m.def((std::string("calc_edge_extensions_")+std::to_string(DIM)+std::string("D")).c_str(), &calc_edge_extensions<DIM>, 
         py::arg("disp"), py::arg("net"), py::arg("embed"), py::arg("is_strain") = false);
        
}

template <int DIM> void init_corner_templates(py::module &m) {
    
    m.def((std::string("construct_corner_complex_")+std::to_string(DIM)+std::string("D")).c_str(), 
          &construct_corner_complex<DIM>);
    
    m.def((std::string("find_corners_")+std::to_string(DIM)+std::string("D")).c_str(), 
          &find_corners<DIM>);
    m.def((std::string("calc_corner_strains_")+std::to_string(DIM)+std::string("D")).c_str(), 
          &calc_corner_strains<DIM>);
    
        
}

#ifdef ALPHA
template <int DIM> void init_alpha_templates(py::module &m) {
    
    
    m.def((std::string("construct_alpha_complex_")+std::to_string(DIM)+std::string("D")).c_str(), &construct_alpha_complex<DIM>,
         py::arg("NV"), py::arg("embed"), py::arg("weights"), py::arg("oriented")=false);
    
    m.def((std::string("calc_alpha_vals_")+std::to_string(DIM)+std::string("D")).c_str(), &calc_alpha_vals<DIM>,
         py::arg("comp"), py::arg("embed"), py::arg("weights"), py::arg("alpha0")=-1.0);
    
    m.def((std::string("calc_strains_")+std::to_string(DIM)+std::string("D")).c_str(), &calc_strains<DIM>);
    
    
    m.def((std::string("calc_voronoi_D2min_")+std::to_string(DIM)+std::string("D")).c_str(), &calc_voronoi_D2min<DIM>);
}
#endif

template <int DIM> void init_search_templates(py::module &m) {

    m.def((std::string("calc_euclid_pair_dists")+std::to_string(DIM)+std::string("D")).c_str(), 
          &calc_euclid_pair_dists<DIM>);
    
    m.def((std::string("calc_euclid_point_dists")+std::to_string(DIM)+std::string("D")).c_str(), 
          &calc_euclid_point_dists<DIM>);
    
    m.def((std::string("get_point_neighborhood")+std::to_string(DIM)+std::string("D")).c_str(), 
          &get_point_neighborhood<DIM>);


        
}


PYBIND11_MODULE(chomology, m) {
    
    // Cell complex
    
    py::class_<CellComplex>(m, "CellComplex")
        .def_readonly("dim", &CellComplex::dim)
        .def_readonly("ncells", &CellComplex::ncells)
        .def_readonly("regular", &CellComplex::regular)
        .def_readonly("oriented", &CellComplex::oriented)
        .def(py::init<int, bool, bool>(), py::arg("dim"), py::arg("regular")=true, py::arg("oriented")=false)
        .def("add_cell", (void (CellComplex::*)(int, int, std::vector<int>&, std::vector<int>&)) &CellComplex::add_cell)
        .def("get_dim", &CellComplex::get_dim)
        .def("get_label", &CellComplex::get_label)
        .def("get_facets", &CellComplex::get_facets)
        .def("get_cofacets", &CellComplex::get_cofacets)
        .def("get_coeffs", &CellComplex::get_coeffs)
        .def("get_faces", &CellComplex::get_faces, py::arg("alpha"), py::arg("target_dim")=-1)
        .def("get_cofaces", &CellComplex::get_cofaces, py::arg("alpha"), py::arg("target_dim")=-1)
        .def("get_star", &CellComplex::get_star, py::arg("alpha"), py::arg("dual"), py::arg("target_dim")=-1)
        .def("get_labels", &CellComplex::get_labels)
        .def("make_compressed", &CellComplex::make_compressed)
        .def("construct_cofacets", &CellComplex::construct_cofacets);
        
    //     m.def("check_boundary_op", &check_boundary_op, 
    //           "Checks the boundary operator of a complex to ensure that \\delta_d\\delta_(d-1) = 0 for each cell.");
    //     m.def("get_boundary", &get_boundary);

    
    // Filtration
    
    py::class_<Filtration>(m, "Filtration")
        .def_readonly("ncells", &Filtration::ncells)
        .def_readonly("ascend", &Filtration::ascend)
        .def(py::init<CellComplex&, std::vector<double>&, std::vector<int>&, std::vector<int>&, bool, int>(),
             py::arg("comp"), py::arg("func"), py::arg("digi_func"), 
             py::arg("order"), py::arg("ascend")=true, py::arg("filt_dim")=-1)
        .def("get_func", &Filtration::get_func)
        .def("get_digi_func", &Filtration::get_digi_func)
        .def("get_order", &Filtration::get_order)
        .def("get_filtration", &Filtration::get_filtration);
    
    
    m.def("construct_filtration", &construct_filtration,
         py::arg("comp"), py::arg("func"), py::arg("ascend")=true);
    m.def("construct_induced_filtration", &construct_induced_filtration,
         py::arg("comp"), py::arg("func"), py::arg("func_order"),py::arg("filt_dim"), py::arg("ascend")=true);
    //     m.def("perform_watershed_transform", &perform_watershed_transform, 
    //           py::arg("time"), py::arg("comp"), py::arg("ascend")=true, py::arg("co")=false);
    //     
    //     m.def("reduce_filtration", &reduce_filtration);
    
    // Network complex
    
    init_network_templates<1>(m);
    init_network_templates<2>(m);
    
    py::class_<Network>(m, "Network")
        .def(py::init<int, int, std::vector<int>&, std::vector<int>&>())
        .def_readonly("NV", &Network::NV)
        .def_readonly("NE", &Network::NE)
        .def_readonly("edgei", &Network::edgei)
        .def_readonly("edgej", &Network::edgej)
        .def_readonly("neighbors", &Network::neighbors);
    
    m.def("construct_network_complex", &construct_network_complex);

    // Corner complex
    
    init_corner_templates<1>(m);
    init_corner_templates<2>(m);
    
    // Cubical complex
    
    m.def("construct_cubical_complex", &construct_cubical_complex);
    m.def("construct_masked_cubical_complex", &construct_masked_cubical_complex);
    m.def("get_boundary_pixels", &get_boundary_pixels);
    m.def("calc_elongation", &calc_elongation);
    
#ifdef ALPHA
    
    // Alpha complex
    
    init_alpha_templates<2>(m);
    init_alpha_templates<3>(m);
    // init_alpha_templates<4>(m);
    
    m.def("calc_radial_gap_distribution", &calc_radial_gap_distribution,
          py::arg("cell_list"), py::arg("alpha_vals"), py::arg("comp"), py::arg("max_dist")=-1, py::arg("verbose")=false);
    
    m.def("calc_angular_gap_distribution", &calc_angular_gap_distribution,
          py::arg("cell_list"), py::arg("alpha_vals"), py::arg("comp"), py::arg("max_dist")=-1, py::arg("verbose")=false);
    
    
    m.def("calc_radial_cycle_distribution", &calc_radial_cycle_distribution,
          py::arg("cell_list"), py::arg("alpha_vals"), py::arg("comp"), py::arg("vertex_types"), py::arg("max_dist")=-1, py::arg("verbose")=false);

#endif
    
    
    
    // Morse complex
    
    m.def("get_star_decomp", &get_star_decomp, 
          py::arg("alpha"), py::arg("filt"), py::arg("comp"), py::arg("target_dim")=-1);
    m.def("construct_discrete_gradient", &construct_discrete_gradient);
    m.def("traverse_flow", &traverse_flow, py::arg("s"), py::arg("V"), 
          py::arg("comp"), py::arg("co")=true, py::arg("coordinated")=false);
    m.def("find_morse_boundary", &find_morse_boundary, py::arg("s"), py::arg("V"), 
          py::arg("comp"), py::arg("co")=true, py::arg("oriented")=false);
    m.def("construct_morse_complex", &construct_morse_complex, py::arg("V"), 
          py::arg("comp"),  py::arg("oriented")=false);
    m.def("find_connections", &find_connections);
    
    // Morse feature extraction
    m.def("convert_morse_to_original", &convert_morse_to_original,
         py::arg("s"), py::arg("V"), py::arg("morse_comp"), py::arg("comp"), py::arg("co")=true);
    
    m.def("find_morse_basins", &find_morse_basins);
    m.def("find_morse_skeleton", &find_morse_skeleton,
         py::arg("V"), py::arg("morse_comp"), py::arg("comp"), py::arg("skeleton_dim")=1);
    
    m.def("convert_feature_dim", &convert_feature_dim,
          py::arg("feature"), py::arg("target_dim"), py::arg("filt"), py::arg("comp"), py::arg("inclusive")=true);


    //     m.def("find_morse_basin_borders", &find_morse_basin_borders,
    //          py::arg("mcomp"), py::arg("V"), py::arg("coV"), py::arg("filt"), py::arg("comp"), py::arg("target_dim")=-1);
    //     m.def("extract_morse_basin", &extract_morse_basin, 
    //          py::arg("s"), py::arg("mcomp"), py::arg("V"), py::arg("coV"), 
    //           py::arg("comp"), py::arg("filt"), py::arg("target_dim")=-1);
    //     m.def("extract_morse_feature", &extract_morse_feature, 
    //          py::arg("i"), py::arg("j"), py::arg("mcomp"), py::arg("filt"), py::arg("target_dim")=-1,
    //     py::arg("complement")=false);
    //     m.def("extract_morse_feature_to_real", &extract_morse_feature_to_real, 
    //          py::arg("i"), py::arg("j"), py::arg("mcomp"), py::arg("V"), py::arg("coV"), 
    //           py::arg("comp"), py::arg("filt"), py::arg("complement")=false, py::arg("target_dim")=-1);
    
    
    
    
    // Cell complex searching
    
    
    init_search_templates<2>(m);
    init_search_templates<3>(m);
    
    m.def("calc_comp_pair_dists", &calc_comp_pair_dists);
    m.def("calc_comp_point_dists", &calc_comp_point_dists,
         py::arg("p"), py::arg("comp"), py::arg("max_dist")=-1);
    m.def("find_nearest_neighbors", &find_nearest_neighbors);
    //     m.def("find_thresholded_component", &find_thresholded_component);
    

    
    // Persistent homology
    
        
    m.def("calc_extended_persistence", (std::tuple<std::tuple<std::vector<std::pair<int, int> >, 
                                        std::vector<std::pair<int, int> >, 
                                        std::vector<std::pair<int, int> > >,
                                        std::unordered_map<int, std::vector<int> > > (*)
                                        (Filtration&, Filtration&, CellComplex&, bool, int)) &calc_extended_persistence,
                                     py::arg("filt_asc"), py::arg("filt_desc"), py::arg("comp"),
                                     py::arg("ext_cycles"), py::arg("dim")=-1);
    
    m.def("calc_extended_persistence", (std::tuple<std::vector<std::pair<int, int> >, 
                                        std::vector<std::pair<int, int> >, 
                                        std::vector<std::pair<int, int> > > (*)
                                        (Filtration&, Filtration&, CellComplex&)) &calc_extended_persistence,
                                     py::arg("filt_asc"), py::arg("filt_desc"), py::arg("comp"));
    
    
    m.def("calc_birth_cycles", &calc_birth_cycles, py::arg("filt"), py::arg("comp"), py::arg("dim")=-1);
    m.def("calc_homologous_birth_cycles", &calc_homologous_birth_cycles, py::arg("filt"), py::arg("comp"), py::arg("dim")=-1);
    
    
    
#ifdef OPTIMAL
    
    // Optimal cycles
    
    m.def("calc_optimal_cycles", &calc_optimal_cycles, 
          py::arg("filt"), py::arg("comp"), py::arg("weights"), py::arg("dim")=-1, py::arg("verbose")=false,
         py::call_guard<py::scoped_ostream_redirect,
                     py::scoped_estream_redirect>());

    //     m.def("calc_optimal_homologous_cycles", &calc_optimal_homologous_cycles, 
    //           py::arg("filt"), py::arg("comp"), py::arg("weights"), py::arg("dim")=-1, py::arg("verbose")=false,
    //          py::call_guard<py::scoped_ostream_redirect,
    //                      py::scoped_estream_redirect>());
#endif
        
    
    
    // Persistence simplification

    m.def("simplify_morse_complex", &simplify_morse_complex, 
          py::arg("threshold"), py::arg("V"), py::arg("coV"), py::arg("comp"),
          py::arg("insert_order"), py::arg("leq") = true, py::arg("verbose") = false);
    m.def("find_cancel_threshold", &find_cancel_threshold);
    m.def("find_join_threshold", &find_join_threshold);
    
    
    
    

    
    
//
    

    
//     m.def("extract_persistence_feature", &extract_persistence_feature, 
//          py::arg("i"), py::arg("j"), py::arg("comp"), py::arg("filt"), py::arg("target_dim")=-1, py::arg("complement")=false);
    
//     // m.def("calc_persistence_landscape", (std::vector<std::vector<double> > (*)(std::vector<double>&, std::vector<double>&, std::vector<double>&, int)) &calc_persistence_landscape);
//     // m.def("calc_persistence_landscape", (std::vector<std::vector<double> > (*)(std::vector<std::pair<int, int> >&, std::vector<double>&, int, StarFiltration&)) &calc_persistence_landscape);
    
//     m.def("calc_landscape", &calc_landscape, py::return_value_policy::take_ownership);
//     m.def("calc_landscape_norm", (double (*)(CRXMat, CRXVec))&calc_landscape_norm);
//     m.def("calc_landscape_dist", (double (*)(CRXMat, CRXMat, CRXVec))&calc_landscape_dist);
//     m.def("calc_landscape_norm", (double (*)(CRXVec, CRXVec, CRXVec))&calc_landscape_norm);
//     m.def("calc_landscape_dist", (double (*)(CRXVec, CRXVec, CRXVec, CRXVec, CRXVec))&calc_landscape_dist);
    
    
    

        

     
};




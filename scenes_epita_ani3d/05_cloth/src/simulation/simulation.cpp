#include "simulation.hpp"

using namespace cgp;




// Fill value of force applied on each particle
// - Gravity
// - Drag
// - Spring force
// - Wind force
void simulation_compute_force(cloth_structure& cloth, simulation_parameters const& parameters)
{
    // direct access to the variables
    grid_2D<vec3>& force = cloth.force;
    grid_2D<vec3> const& position = cloth.position;
    grid_2D<vec3> const& velocity = cloth.velocity;

    size_t const N = cloth.position.size();       // total number of vertices
    size_t const N_edge = cloth.N_samples_edge(); // number of vertices in one dimension of the grid

    // Retrive simulation parameter
    float const K = parameters.K;              // spring stifness
    float const m = parameters.mass_total / N; // mass of a particle
    float const mu = parameters.mu;            // damping coefficient
    float const	L0 = 1.0f / (N_edge - 1.0f);   // rest length between two direct neighboring particle


    // Gravity
    const vec3 g = { 0,0,-9.81f };
    for (int ku = 0; ku < N_edge; ++ku)
        for (int kv = 0; kv < N_edge; ++kv)
            force(ku, kv) = m * g;

    // Drag
    for (int ku = 0; ku < N_edge; ++ku)
        for (int kv = 0; kv < N_edge; ++kv)
            force(ku, kv) += -mu * m * velocity(ku, kv);


    // Spring forces
    const vec2 offsets[] = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1},   // structural
        {-1, 1}, {1, 1}, {1, -1}, {-1, -1}, // shearing
        {-2, 0}, {0, 2}, {2, 0}, {0, -2}    // bending
    };

    for (int ku = 0; ku < N_edge; ++ku)
    {
        for (int kv = 0; kv < N_edge; ++kv)
        {
            cgp::vec3 springForces = { 0, 0, 0 };
            cgp::vec3 pos = cloth.position(ku, kv);
            for (const vec2& off : offsets)
            {
                if (ku + off.x < 0 || kv + off.y < 0 || ku + off.x >= N_edge || kv + off.y >= N_edge)
                    continue;

                cgp::vec3 p = cloth.position(ku + off.x, kv + off.y);
                const auto dist = norm(p - pos);
                springForces += K * (dist - L0 * norm(off)) * ((p - pos) / dist);
            }
            force(ku, kv) += springForces;
        }
    }

    // Wind force
    for (int ku = 0; ku < N_edge; ++ku)
    {
        for (int kv = 0; kv < N_edge; ++kv)
        {
            auto pos = cloth.position(ku, kv);
            auto normal = cloth.normal(ku, kv);
            force(ku, kv) += dot(normal, parameters.wind.direction) * normal * parameters.wind.magnitude;
        }
    }
}

void simulation_numerical_integration(cloth_structure& cloth, simulation_parameters const& parameters, float dt)
{
    int const N_edge = cloth.N_samples_edge();
    float const m = parameters.mass_total/ static_cast<float>(N_edge);

    for (int ku = 0; ku < N_edge; ++ku) {
        for (int kv = 0; kv < N_edge; ++kv) {
            vec3& v = cloth.velocity(ku, kv);
            vec3& p = cloth.position(ku, kv);
            vec3 const& f = cloth.force(ku, kv);

            // Standard semi-implicit numerical integration
            v = v + dt * f / m;
            p = p + dt * v;
        }
    }
    
}

void simulation_apply_constraints(cloth_structure& cloth, constraint_structure const& constraint)
{
    // Fixed positions of the cloth
    for (auto const& it : constraint.fixed_sample) {
        position_contraint c = it.second;
        cloth.position(c.ku, c.kv) = c.position; // set the position to the fixed one
    }

    // To do: apply external constraints
    // For all vertex:
    //   If vertex is below floor level ...
    //   If vertex is inside collision sphere ...

    const float ground = constraint.ground_z;
    const int edges = cloth.N_samples_edge();
    for (int ku = 0; ku < edges; ku++)
    {
        for (int kv = 0; kv < edges; kv++)
        {
            auto pos = cloth.position(ku, kv);
        }
    }
}



bool simulation_detect_divergence(cloth_structure const& cloth)
{
    bool simulation_diverged = false;
    const size_t N = cloth.position.size();
    for (size_t k = 0; simulation_diverged == false && k < N; ++k)
    {
        const float f = norm(cloth.force.data.at_unsafe(k));
        const vec3& p = cloth.position.data.at_unsafe(k);

        if (std::isnan(f)) // detect NaN in force
        {
            std::cout << "\n **** NaN detected in forces" << std::endl;
            simulation_diverged = true;
        }

        if (f > 600.0f) // detect strong force magnitude
        {
            std::cout << "\n **** Warning : Strong force magnitude detected " << f << " at vertex " << k << " ****" << std::endl;
            simulation_diverged = true;
        }

        if (std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z)) // detect NaN in position
        {
            std::cout << "\n **** NaN detected in positions" << std::endl;
            simulation_diverged = true;
        }
    }

    return simulation_diverged;
}


#include "simulation.hpp"

using namespace cgp;



void simulate(std::vector<particle_structure>& particles, float dt)
{
	float const Epsilon = 0.1f;
	vec3 const g = { 0,0,-9.81f };
	size_t const N = particles.size();

	vec3 plane = { 0, 0, -1.1 };
	vec3 normal = { 0, 0, 1 };

	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j) {
			if (i == j)
				continue;

			particle_structure& p1 = particles[i];
			particle_structure& p2 = particles[j];

			float dist = norm(p1.p - p2.p);
			if (dist == 0)
				continue;

			vec3 u = normalize(p1.p - p2.p);
			if (dist <= p1.r + p2.r) // Sphere collision
			{
				float depth = p1.r + p2.r - dist;
				p1.p += depth / (2 * u);
				p2.p -= depth / (2 * u);

				float speed_diff = norm(p2.v - p1.v);
				if (speed_diff <= Epsilon)
				{
					// static contact
					p1.v *= 0.8f;
					p2.v *= 0.8f;
				}
				else
				{
					// collision
					p1.v += dot(p2.v - p1.v, u) * u;
					p2.v -= dot(p2.v - p1.v, u) * u;
				}
			}
		}
	}

	for (size_t k = 0; k < N; ++k)
	{
		particle_structure& particle = particles[k];

		float detection = dot(particle.p - plane, normal);
		if (detection <= particle.r)
		{
			vec3 v_impact = dot(particle.v, normal) * normal;
			vec3 v_friction = particle.v - dot(particle.v, normal) * normal;
			vec3 new_v = 0.8f * v_friction - 0.8f * v_impact;

			float dist = particle.r - dot(particle.p - plane, normal);
			vec3 new_p = particle.p + dist * normal;

			particle.p = new_p;
			particle.v = new_v;
		}
		else
		{
			vec3 const f = particle.m * g;

			particle.v = (1 - 0.9f * dt) * particle.v + dt * f;
			particle.p = particle.p + dt * particle.v;
		}
	}
}

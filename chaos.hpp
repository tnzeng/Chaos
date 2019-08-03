#ifndef __CHAOS_HPP__
#define __CHAOS_HPP__

#include<vector>
#include<cmath>
#include<random>
#include<chrono>

#include "point.hpp"

class Polygon{
	private:
		int n;
		std::vector<Point> vertices;
	public:
		Polygon(){
		}
		
		Polygon(int sides, std::vector<Point> v){
			n = sides;
			vertices = v;
		}

		Polygon (int n){
			set_regular(n);
		}

		void set_regular(int sides){
			vertices.clear();
			n = sides;
			//TODO: some way to deal w/ PI?
			double ang = 3.14159265358979 * 2 / n;
			for (int i = 0; i < n; i++){
				Point p (cos(ang * i), sin(ang * i));
				vertices.push_back(p);
			}
		}

		void set_vertices(std::vector<Point> v){
			n = (int) v.size();
			vertices = v;
		}

		int get_n(){
			return n;
		}

		Point get_vertex(int i){
			return vertices[i];
		}

		void print(){
			std::cout << "POLYGON: " << std::endl;
			for (int i = 0; i < n; i++)
				vertices[i].print();
		}
};

class Ruleset{
	private:
		std::vector<int> choices;
	public:
		Ruleset(){
		}

		Ruleset(std::vector<int> c){
			choices = c;
		}

		void set_choices(std::vector<int> c){
			choices = c;
		}

		int get_size(){
			return (int) choices.size();
		}

		int get_choice(int i){
			return choices[i];
		}

		bool contains(int x){
			for (unsigned int i = 0; i < choices.size(); i++)
				if (choices[i] == x)
					return true;
			return false;
		}

		void print(){
			std::cout << "RULESET: " << std::endl;
			for (unsigned int i = 0; i < choices.size(); i++)
				std::cout << choices[i] << " ";
			std::cout << std::endl;
		}
};

class Chaos{
	private:
		Polygon polygon;
		Ruleset rules;
		int choice;
		Point point;

		std::default_random_engine generator;
		std::uniform_int_distribution<int> dist;
	public:
		Chaos(){
		}

		Chaos(Ruleset r, Polygon p){
			choice = 0;
			rules = r;
			polygon = p;
			point = create_point(0, 0);
			
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine g (seed);
			std::uniform_int_distribution<int> d (0, (int) rules.get_size() - 1);
//			std::cout << "HELP" << std::endl;
			generator = g;
			dist = d;
		}

		void next_point(){
			int rand = rules.get_choice(dist(generator));
//			if (!rules.contains(rand))
//				std::cout << "WTF" << std::endl;
			choice = (choice + rand) % polygon.get_n();
//			std::cout << "RAND: " << rand << ", CHOICE: " << choice << ", N: " << polygon.get_n() << std::endl;
			point = scale_point(0.5, add_point(point, polygon.get_vertex(choice)));
		}

		Point get_point(){
			return point;
		}

		void print(){
			polygon.print();
			rules.print();
		}
};

#endif

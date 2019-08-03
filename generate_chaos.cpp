#include<iostream>
#include<vector>
#include<string>
#include<cmath>

#include "bitmap.hpp"
#include "cartesian_histogram.hpp"
#include "color.hpp"
#include "chaos.hpp"
#include "command_flags.hpp"
#include "point.hpp"

std::vector<std::vector<std::string>> interpret_flags(int argc, char** argv){
	Flags f;
	f.add_flag("-f", 1); //file name
	f.add_flag("-w", 4); //window
	f.add_flag("-s", 1); //pixel size
	f.add_flag("-p", -1); //points
	f.add_flag("-n", 1); //regular polygon
	f.add_flag("-r", -1); //ruleset
	f.add_flag("-b", 1); //bound

	return f.read_flags(argc, argv);
}

Color color_map(int z, int max, Point point){
	Color purple = create_color(155, 0, 255);
	Color gold = create_color(255, 255, 0);
	double factor = (double) z / max;
	double scaled = pow(factor, 0.4);
	double dist = sqrt(point.x * point.x + point.y * point.y);
	Color color = add_color(scale_color(purple, dist), scale_color(gold, 1 - dist));
	return scale_color(color, scaled);
}

std::vector<std::vector<Color>> generate_histogram(Chaos c, double x_lower_bound, double y_lower_bound, double x_upper_bound, double y_upper_bound, double pixel_size, int image_width, int image_height, int bound){
//	c.print();
	Cartesian_Histogram cart (x_lower_bound, y_lower_bound, x_upper_bound, y_upper_bound, pixel_size, image_width, image_height);
	cart.set_color_map(color_map);
	for (int i = 0; i < bound; i++){
		c.next_point();
		cart.increment_point(c.get_point());
	}
	int max = cart.find_histogram_max();
	cart.histogram_to_color_data(max);
	return cart.get_color_data();
}

void generate_chaos(std::vector<std::vector<std::string>> flag_args){
	std::string file_name ("test.bmp");
	if (flag_args[0].size() > 0)
		file_name = flag_args[0][0];
	double x_lower_bound = -1;
	double y_lower_bound = -1;
	double x_upper_bound = 1; 
	double y_upper_bound = 1;
	if (flag_args[1].size() > 0){
		x_lower_bound = std::stod(flag_args[1][0]);
		y_lower_bound = std::stod(flag_args[1][1]);
		x_upper_bound = std::stod(flag_args[1][2]);
		y_upper_bound = std::stod(flag_args[1][3]);
	}
	double pixel_size = 0.1;
	if (flag_args[2].size() > 0)
		pixel_size = std::stod(flag_args[2][0]);
	Polygon p (3);
	if (flag_args[3].size() > 0){
		std::vector<Point> v;
		for (unsigned int i = 0; i < flag_args[3].size() / 2; i++){
			v.push_back(create_point(std::stod(flag_args[3][2 * i]), std::stod(flag_args[3][2 * i + 1])));
		}
		p.set_vertices(v);
	}
	if (flag_args[4].size() > 0)
		p.set_regular(std::stoi(flag_args[4][0], nullptr, 10));
	std::vector<int> c;
	c.push_back(0); c.push_back(1); c.push_back(p.get_n() - 1);
	Ruleset r (c);
	if (flag_args[5].size() > 0){
		c.clear();
		for (unsigned int i = 0; i < flag_args[5].size(); i++)
			c.push_back(std::stoi(flag_args[5][i], nullptr, 10));
		r.set_choices(c);
	}
	int bound = 1000000;
	if (flag_args[6].size() > 0)
		bound = std::stoi(flag_args[6][0], nullptr, 10);

	int image_width = (int) floor((x_upper_bound - x_lower_bound) / pixel_size);
	int image_height = (int) floor((y_upper_bound - y_lower_bound) / pixel_size);
	Bitmap bitmap (file_name, image_width, image_height);

	Chaos chaos (r, p);

	bitmap.set_color_data(generate_histogram(chaos, x_lower_bound, y_lower_bound, x_upper_bound, y_upper_bound, pixel_size, image_width, image_height, bound));
	bitmap.write();
}

int main(int argc, char** argv){
	std::vector<std::vector<std::string>> flag_args = interpret_flags(argc, argv);
	generate_chaos(flag_args);

	return 0;
}

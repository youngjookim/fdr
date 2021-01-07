#pragma float_control( except, on )
#include <tapkee/tapkee.hpp>
#include <tapkee/callbacks/precomputed_callbacks.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include "define.hpp"
#include "nanoflann.h"
#include "Eigen/Dense"
#include <random>
#include <filesystem>
#include <windows.h>
using namespace std;
 
void read_obs_dim(int* obs, int* dim, const char * file_str); // later: throw / catch --> readfile fail process instead of exit() or return
void read_file(double* dst_arr, const char* file_str);
void write_file(tapkee::DenseMatrix &dst_arr, const char* file_str, int obs, int aim_dim);
void write_file(float* dst_arr, const char* file_str, int obs, int aim_dim);
void write_file(double* dst_arr, const char* file_str, int obs, int aim_dim);
double get_wall_time();
void sharpening_for_dr(double* data, int obs, int dim, double lambda1);
void copy_data(double* arr_src, tapkee::DenseMatrix& arr_dst, int& obs, int& dim);
int main()
{
	int idx3 = 0;

	double max1 = 0, min1 = 0;
	double t = 0;
	double* data = 0;		//input data
	int obs = 0, dim = 0;	//{obs, dim}
	int idx1 = 0, idx2 = 0;
	int endCMD = 0;
	int aim_dim = 2;
	const char * file_str_header = "data__header.txt";	//input data header
	const char * file_str_original_data = "data_.txt";	//input data file
	const char * file_str_dr = "data__result_dr.txt";	//output data for dr
	const char * file_str_s_dr = "data__s_dr.txt";		//output data for sdr
	const char * file_str_info = "info.txt";			//output data or etc. info
	const char * file_str_lgc = "data__lgc.txt";		//output data for lgc

	std::string fullpath;
	std::string cur_path;

	for (auto & p_par : std::experimental::filesystem::directory_iterator(PATH)) //Run for different data sets in each directory
	{
			ofstream in_file;
			cur_path = p_par.path().string().c_str();//Read header
			cur_path += "\\"; 
			fullpath = cur_path + file_str_info;
			in_file.open(fullpath);
			in_file << "Learning rate: " << LEARNING_RATE << endl;
			in_file << "Tapkee DR used: " << TAPKEE_NUM_DR << endl;
			if (in_file.fail())
			{
				cerr << "    (ERROR) main(): Failed to open log file" << endl;
				return 0;
			}
			std::cout << "- DIR: " << p_par.path().string().c_str() << endl;
			fullpath = cur_path + file_str_header;
			read_obs_dim(&obs, &dim, fullpath.c_str());	//read header
			if(obs>0 && dim>1)
				data = new double[obs * dim];
			fullpath = cur_path + file_str_original_data;
			read_file(data, fullpath.c_str()); //read data and copy it to destination memory
			tapkee::DenseMatrix data_mat(obs, dim);

			for (idx1 = 0; idx1 < dim; idx1++)//Normalize between 0 and 1
			{
				max1 = data[idx1];
				min1 = data[idx1];
				for (idx2 = 1; idx2 < obs; idx2++)
				{
					max1 = max(data[idx2*dim + idx1], max1);
					min1 = min(data[idx2*dim + idx1], min1);
				}
				for (idx2 = 0; idx2 < obs; idx2++)
					data[idx2*dim + idx1] = ((data[idx2*dim + idx1] - min1) / (max1 - min1));
			}
			in_file << endl;
#if DR_MODE
			copy_data(data, data_mat, obs, dim); // later: should improve code without copy(s)
			t = get_wall_time();
			tapkee::TapkeeOutput output1 = tapkee::initialize()
				.withParameters((tapkee::method = (static_cast<tapkee::DimensionReductionMethod>(TAPKEE_NUM_DR)), //tapkee::method = tapkee::tDistributedStochasticNeighborEmbedding
					tapkee::num_neighbors = TAPKEE_NUM_NEIGHBORS,	//default=10
					//tapkee::sne_perplexity = tsne_perplexity,		//default=30
					tapkee::target_dimension = aim_dim))			//.withDistance(distance)
				.embedUsing(data_mat.transpose());					//.withFeatures(data_mat);
			t = get_wall_time() - t;
			in_file << "Wall-clock time of vanilla DR: " << t << endl;
			std::cout << "    " << "DR: " << t << " sec    " << endl;
			fullpath = cur_path + file_str_dr;
			write_file(output1.embedding, fullpath.c_str(), obs, aim_dim);
#endif
#if SDR_MODE
			///lgc step
			fullpath = cur_path + file_str_lgc;
			t = get_wall_time();
			if (LEARNING_RATE)
				sharpening_for_dr(data, obs, dim, LEARNING_RATE);
			else
				std::cout << "    Check learning rate parameter for LGC." << endl;
			t = get_wall_time() - t;
			in_file << "Wall-clock time of LGC step of SDR: " << t << endl;
			std::cout << "    LGC step" << ": " << t << "sec" << endl;
			write_file(data, fullpath.c_str(), obs, dim);
			///dr step
			copy_data(data, data_mat, obs, dim);
			t = get_wall_time();
			tapkee::TapkeeOutput output2 = tapkee::initialize()
				.withParameters((tapkee::method = (static_cast<tapkee::DimensionReductionMethod>(TAPKEE_NUM_DR)), //tapkee::method = tapkee::tDistributedStochasticNeighborEmbedding
					tapkee::num_neighbors = TAPKEE_NUM_NEIGHBORS,	//default=10
					//tapkee::sne_perplexity = tsne_perplexity,		//default=30
					tapkee::target_dimension = aim_dim))			//.withDistance(distance)
				.embedUsing(data_mat.transpose());					//.withFeatures(data_mat);
			t = get_wall_time() - t;
			in_file << "Wall-clock time of DR step of SDR: " << t << endl;
			std::cout << "    DR step: " << t << "sec" << endl;
			fullpath = cur_path + file_str_s_dr;
			write_file(output2.embedding, fullpath.c_str(), obs, aim_dim);
#endif
			in_file.close();
			if (obs > 0 && dim > 1)
				delete[] data;
	}
	std::cout << endl << "Press any key & Enter to end the program..." << endl;
	std::cin >> endCMD;
	return 0;
}
void read_obs_dim(int* obs, int* dim, const char* file_str) //OVERLOAD
{
	ifstream in_file;
	int idx = 0;
	in_file.open(file_str);
	if (in_file.fail())
	{
		cerr << "(ERROR) read_input_data(): failed to open file" << endl;
		return;
	}
	while (!in_file.eof())
		in_file >> *obs >> *dim;
	in_file.close();
}
void read_file(double* dst_arr, const char* file_str) //Read input data file (rows/lines: observations, columns: dimensions/attributes)
{
	ifstream in_file;
	int idx = 0;
	in_file.open(file_str);
	if (in_file.fail())
	{
		cerr << "(ERROR) read_input_data(): failed to open file" << endl;
		return;
	}
	while (!in_file.eof())
	{
		in_file >> dst_arr[idx]; //Copy each entry of matrix
		idx++;
	}
	in_file.close();
}
void write_file(tapkee::DenseMatrix &dst_arr, const char* file_str, int obs, int aim_dim)
{
	ofstream in_file;
	int idx1 = 0;
	int idx2 = 0;
	in_file.open(file_str);
	if (in_file.fail())
	{
		cerr << "(ERROR) write_input_data(): failed to open file" << endl;
		return;
	}
	for (int idx1 = 0; idx1 < obs; idx1++)
	{
		for (int idx2 = 0; idx2 < aim_dim; idx2++)
			in_file << dst_arr(idx1, idx2) << " ";
		in_file << "\n";
	}
	in_file.close();
}
void write_file(float* dst_arr, const char* file_str, int obs, int aim_dim)
{
	ofstream in_file;
	int idx1 = 0;
	int idx2 = 0;
	in_file.open(file_str);
	if (in_file.fail())
	{
		cerr << "(ERROR) write_input_data(): failed to open file" << endl;
		return;
	}
	for (int idx1 = 0; idx1 < obs; idx1++)
	{
		for (int idx2 = 0; idx2 < aim_dim; idx2++)
			in_file << dst_arr[aim_dim * idx1 + idx2] << ", ";
		in_file << "\n";
	}
	in_file.close();
}
void write_file(double* dst_arr, const char* file_str, int obs, int aim_dim)
{
	ofstream in_file;
	int idx1 = 0;
	int idx2 = 0;
	in_file.open(file_str);
	if (in_file.fail())
	{
		cerr << "(ERROR) write_input_data(): failed to open file" << endl;
		return;
	}
	for (int idx1 = 0; idx1 < obs; idx1++)
	{
		for (int idx2 = 0; idx2 < aim_dim; idx2++)
			in_file << dst_arr[aim_dim * idx1 + idx2] << " ";
		in_file << "\n";
	}
	in_file.close();
}
double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}
void sharpening_for_dr(double* data, int obs, int dim, double lambda1)
{
	typedef nanoflann::KDTreeEigenMatrixAdaptor<Eigen::MatrixXd> KDTree;
	int idx1 = 0, idx2 = 0;
	int flag = 0;
	double h1 = 0;
	Eigen::MatrixXd gradient_rho(1, dim);
	Eigen::MatrixXd ptCloud(obs, dim);
	Eigen::MatrixXd shift(1, dim);
	Eigen::MatrixXd tmp_location(1, dim);
	size_t nMatches = 0;

	/// Memory allocation
	double* location = new double[dim];
	double* out_dist_sqr_tmp = new double[KNN_K + 1];
	Eigen::Index* ret_index_tmp = new Eigen::Index[KNN_K + 1];

	/// Assign and fill
	std::fill((Eigen::Index*)ret_index_tmp, (Eigen::Index*)(ret_index_tmp + KNN_K + 1), 0);
	std::fill((double*)out_dist_sqr_tmp, (double*)(out_dist_sqr_tmp + KNN_K + 1), 0);
	for (idx1 = 0; idx1 < obs; idx1++)//Initialize - put data into ptCloud
	{
		for (idx2 = 0; idx2 < dim; idx2++)
			ptCloud(idx1, idx2) = data[dim * idx1 + idx2];
	}

	/// FLMDS
	cout << "    LGC in progress:";
	while (flag < NUM_ITR)
	{
		cout << "    " << flag << " ";
		KDTree index(dim, ptCloud, 10);//10=#max leaf
		index.index->buildIndex();
		for (idx1 = 0; idx1 < obs; idx1++)//for all observations
		{
			for (idx2 = 0; idx2 < dim; idx2++)//each observation is the query point
				location[idx2] = ptCloud(idx1, idx2);
			nMatches = index.index->knnSearch(&location[0], KNN_K + 1, &ret_index_tmp[0], &out_dist_sqr_tmp[0]);
			h1 = out_dist_sqr_tmp[nMatches - 1];
			if (h1 != 0.0)//run only when h1 is non-zero (h1)
			{
				Eigen::MatrixXd neighbors_of_pt(nMatches, dim);
				for (idx2 = 0; idx2 < nMatches; idx2++)		// KNN SEARCH 4lines save the index to the neighbors memory
					neighbors_of_pt.row(idx2) = ptCloud.row(ret_index_tmp[idx2]);
				for (idx2 = 0; idx2 < dim; idx2++) // save current point (point that looks at nearest neighbors) to tmp_location
					tmp_location(0, idx2) = location[idx2];
				for (idx2 = 0; idx2 < dim; idx2++)//Initialize
				{
					gradient_rho(0, idx2) = 0;
					shift(0, idx2) = 0;
				}
				for (idx2 = 0; idx2 < nMatches; idx2++) //for all the neighbors do summation
				{
					gradient_rho = gradient_rho - (((tmp_location - neighbors_of_pt.row(idx2)).array() * 2)
						/ (h1 * h1)).matrix(); // h is the distance to the k nearest neighbor of current point
				}
				shift = gradient_rho / max(gradient_rho.norm(), EPSIL);
				ptCloud.row(idx1) = ptCloud.row(idx1) + (shift * lambda1);
			}
			else
			{
				cout << "-";
			}
			//finish loop for all observations
		}
		flag++;
	}
	cout << endl;
	for (idx1 = 0; idx1 < obs; idx1++)//copy ptCloud into data
	{
		for (idx2 = 0; idx2 < dim; idx2++)
			data[dim * idx1 + idx2] = ptCloud(idx1, idx2);
	}
	delete[] out_dist_sqr_tmp;
	delete[] ret_index_tmp;
	delete[] location;
}
void copy_data(double* arr_src, tapkee::DenseMatrix& arr_dst, int& obs, int& dim)
{
	int idx1 = 0, idx2 = 0;
	for (idx1 = 0; idx1 < obs; idx1++) // copy data to data_mat
	{
		for (idx2 = 0; idx2 < dim; idx2++)
			arr_dst(idx1, idx2) = (double)arr_src[idx1*dim + idx2];
	}
}
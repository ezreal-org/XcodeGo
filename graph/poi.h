//
// Created by jayce on 09/11/2016.
//
#ifndef POI_SEMANTIC_H
#define POI_SEMANTIC_H

const int semantic_type_cnt = 6;
enum Semantic_type
{
	school = 0,
	hospital,
	bar,
    company,
    market,
	residence
};

class Poi
{
public:
	Poi(long long id,double pop,Semantic_type type)
	{
		poi_id = id;
		this->pop = pop;
		this->poi_type = type;
	}
	long long get_poi_id()
	{
		return poi_id;
	}
	double get_pop() 
	{
		return pop;
	}
	Semantic_type get_type()
	{
		return this->poi_type;
	}
private:
	long long poi_id;
	double pop;
	Semantic_type poi_type;
};
#endif // POI_SEMANTIC_H

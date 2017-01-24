//
// Created by jayce on 12/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_DRAW_H
#define C_PLUS_PLUS_PRACTICE_DRAW_H

class Draw_Util{
public:
    Draw_Util(double x_min,double x_max,double y_min,double y_max,int d_x,int d_y,int bs)
    {
        this->x_unit = (d_x-2*bs)/(x_max-x_min);
        this->y_unit = (d_y-2*bs)/(y_max-y_min);
        this->x_min = x_min;
        this->y_min = y_min;
        this->border_size = bs;

		this->draw_start_x = 0;
		this->draw_start_y = 0;
		this->zoom = 1;
		this->paint_size_x = d_x;
		this->paint_size_y = d_y;
    }
	int get_zoom()
	{
		return zoom;
	}
	void zoom_plus()
	{
		x_unit *= 2;
		y_unit *= 2;
		draw_start_x *= 2;
		draw_start_y *= 2;
		draw_start_x += paint_size_x / 2;
		draw_start_y += paint_size_y / 2;
		zoom++;
	}
	void zoom_sub()
	{
		x_unit /= 2;
		y_unit /= 2;
		draw_start_x /= 2;
		draw_start_y /= 2;
		draw_start_x -= paint_size_x / 4;
		draw_start_y -= paint_size_y / 4;
		zoom--;
	}
	void left_move()
	{
		draw_start_x -= paint_size_x / 10;
	}
	void right_move()
	{
		draw_start_x += paint_size_x / 10;
	}
	void top_move()
	{
		draw_start_y -= paint_size_y / 10;
	}
	void buttom_move()
	{
		draw_start_y += paint_size_y / 10;
	}
	double get_paint_sizex() 
	{
		return paint_size_x;
	}
	double get_paint_sizey()
	{
		return paint_size_y;
	}
    void convert_to_draw_pos(double &x,double &y)
    {
        x = (x-x_min)*x_unit + border_size -draw_start_x;
		// 对照旧金山地图，发现这里给出的x,y就是画图坐标的xy
        //y = (y_max-y)*y_unit + border_size - draw_start_y;
		y = (y - y_min)*y_unit + border_size - draw_start_y;


    }
private:
    int border_size;
    double x_min;
    double y_min;
    double x_unit;
    double y_unit;
	//以下用于缩放
	double draw_start_x;
	double draw_start_y;
	int zoom; // 1-7 
	double paint_size_x;
	double paint_size_y;
};

#endif //C_PLUS_PLUS_PRACTICE_DRAW_H

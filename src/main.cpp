#include <vector>
#include <fstream>

#include <ctime>

#include "v3.h"
#include "scene_base.h"
#include "image.h"
#include "input_format.h"
#include "quadric.h"

#include <iostream>
#include <typeinfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define CVUI_IMPLEMENTATION
#include "third-party/cvui/cvui.h"

#define WINDOW_NAME "DQUR - Displaying quadrics using raytracing"


int checktype(int type, bool checked1, bool checked2, bool checked3, bool checked4, bool checked5, bool checked6, bool checked7, bool checked8){
  if (checked1 && type!=1){
    return 1;
  }
  if (checked2 && type!=2){
    return 2;
  }
  if (checked3 && type!=3){
    return 3;
  }
  if (checked4 && type!=4){
    return 4;
  }
  if (checked5 && type!=5){
    return 5;
  }
  if (checked6 && type!=6){
    return 6;
  }
  if (checked7 && type!=7){
    return 7;
  }
  if (checked8 && type!=8){
    return 8;
  }
  return type;
}

int main(int argc, const char *argv[])
{
  std::string infile_name = "default_scene.txt";
  std::ifstream infile(infile_name);
  if (!infile) {
    std::cout << infile_name << " doesn't exist.";
    return -1;
  }

  int width, height;
  std::string outfile_name;

  Scene scene;
  initSceneFromFile(infile, &scene, &width, &height, &outfile_name);
  Quadric *quadric = dynamic_cast<Quadric*>(scene.objects_.at(0));

  if (outfile_name.empty()) {
    std::cout << "Unexpected EOF. Corrupt input file?" << std::endl;
    return -1;
  }

  Image image(width, height);

  std::cout << "Rendering..." << std::endl;
  
  time_t start = time(0);
  scene.render(&image, width, height);
  std::cout << "Rendering finished in " << difftime(time(0), start) << " seconds." << std::endl;

  std::cout << "Writing to " << outfile_name << "." << std::endl;
  image.save(outfile_name);


  cv::Mat frame = cv::Mat(648, 1152, CV_8UC3);
  //frame = cv::Scalar(49, 52, 49);
  cv::Mat render = cv::imread("sample_scene.png");

  cv::Mat doubleBuffer = frame.clone();
  int trackbarWidth = 130;

  cvui::init(WINDOW_NAME, 20);

  int type = 3;
  bool checked1 = false;
  bool checked2 = false;
  bool checked3 = true;
  bool checked4 = false;
  bool checked5 = false;
  bool checked6 = false;
  bool checked7 = false;
  bool checked8 = false;

  double avalue = 2;
  double bvalue = 2;
  double cvalue = 2;

  while (true) {
    doubleBuffer.copyTo(frame);

    cvui::image(frame, 10, 10, render);
    cvui::window(frame, frame.cols - 260, 10, 250, 377, "Settings");
    cvui::text(frame, frame.cols - 250, 40, "Type of quadric");
    cvui::checkbox(frame, frame.cols - 250, 60, "Ellipsoid", &checked1);
    cvui::checkbox(frame, frame.cols - 250, 80, "Elliptic paraboloid", &checked2);
    cvui::checkbox(frame, frame.cols - 250, 100, "Hyperbolic paraboloid", &checked3);
    cvui::checkbox(frame, frame.cols - 250, 120, "Elliptic hyperboloid of one sheet", &checked4);
    cvui::checkbox(frame, frame.cols - 250, 140, "Elliptic hyperboloid of two sheets", &checked5);
    cvui::checkbox(frame, frame.cols - 250, 160, "Elliptic cone", &checked6);
    cvui::checkbox(frame, frame.cols - 250, 180, "Elliptic cylinder", &checked7);
    cvui::checkbox(frame, frame.cols - 250, 200, "Hyperbolic cylinder", &checked8);

    cvui::text(frame, frame.cols - 252, 238, "a");
    cvui::trackbar(frame, frame.cols - 240, 225, 220, &avalue, (double)0.0, (double)10.0);
    cvui::text(frame, frame.cols - 252, 273, "b");
    cvui::trackbar(frame, frame.cols - 240, 260, 220, &bvalue, (double)0.0, (double)10.0);
    cvui::text(frame, frame.cols - 252, 306, "c");
    cvui::trackbar(frame, frame.cols - 240, 295, 220, &cvalue, (double)0.0, (double)10.0);

    if (cvui::button(frame, frame.cols - 250, 350, "&Render")) {
      outfile_name = "render_scene.png";
      quadric->change_type(type);
      quadric->change_param_vec_(V3(avalue, bvalue, cvalue));
      scene.objects_.erase(scene.objects_.begin());
      scene.objects_.push_back(quadric);

      Image image(width, height);

      std::cout << "Rendering..." << std::endl;
      
      time_t start = time(0);
      scene.render(&image, width, height);
      std::cout << "Rendering finished in " << difftime(time(0), start) << " seconds." << std::endl;

      std::cout << "Writing to " << outfile_name << "." << std::endl;
      image.save(outfile_name);
      render = cv::imread(outfile_name);
    }

    if (cvui::button(frame, frame.cols - 86, 350, "&Quit")) {
      break;
    }

    type = checktype(type, checked1, checked2, checked3, checked4, checked5, checked6, checked7, checked8);

    checked1 = false;
    checked2 = false;
    checked3 = false;
    checked4 = false;
    checked5 = false;
    checked6 = false;
    checked7 = false;
    checked8 = false;

    switch (type) {
      case 1:
        checked1 = true;
        break;
      case 2:
        checked2 = true;
        break;
      case 3:
        checked3 = true;
        break;
      case 4:
        checked4 = true;
        break;
      case 5:
        checked5 = true;
        break;
      case 6:
        checked6 = true;
        break;
      case 7:
        checked7 = true;
        break;
      case 8:
        checked8 = true;
        break;
      default:
        break;
    }
    cvui::update();

    cv::imshow(WINDOW_NAME, frame);
  }

  return 0;
}
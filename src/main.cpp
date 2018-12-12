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
#define PI 3.14159265


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

Material* get_material(bool lighting, bool checkerboard, bool contours) {
  Material* material = new Material();
  material->cDiffuse = Color(1, 1, 1);
  material->cReflection = Color(1, 1, 1);
  material->cSpecular = Color(1, 1, 1);
  material->kOcclusion = 0;
  material->kReflection = 0;
  
  if (lighting) {
    material->kAmbient = 0.8;
    material->kDiffuse = 0.6;
    material->kSpecular = 1.5;
    material->shininess = 75;
  } else {
    material->kAmbient = 1;
    material->kDiffuse = 0;
    material->kSpecular = 0;
    material->shininess = 0;
  }

  material->type = 1;
  if (checkerboard && contours) {
    material->type = 4;
  } else if (contours) {
    material->type = 3;
  } else if (checkerboard) {
    material->type = 2;
  }
  
  return material;
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


  cv::Mat frame = cv::Mat(660, 1150, CV_8UC3);
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

  bool lighting = true;
  bool checkerboard = false;
  bool contours = false;

  double avalue = 2;
  double bvalue = 2;
  double cvalue = 2;

  double rotationLR = 180;
  double rotationDU = 90;
  double zoom = 30;
  double bboxr = 4;

  while (true) {
    doubleBuffer.copyTo(frame);

    cvui::image(frame, 10, 10, render);
    cvui::window(frame, frame.cols - 260, 10, 250, 165, "Camera");
    cvui::text(frame, frame.cols - 250, 35, "Rotation");
    cvui::text(frame, frame.cols - 250, 55, "L");
    cvui::trackbar(frame, frame.cols - 245, 45, 210, &rotationLR, (double)0.0, (double)360.0);
    cvui::text(frame, frame.cols - 30, 55, "R");
    cvui::text(frame, frame.cols - 250, 85, "D");
    cvui::trackbar(frame, frame.cols - 245, 75, 210, &rotationDU, (double)0.0, (double)180.0);
    cvui::text(frame, frame.cols - 30, 85, "U");
    cvui::text(frame, frame.cols - 250, 115, "Distance");
    cvui::trackbar(frame, frame.cols - 250, 125, 220, &zoom, (double)1.0, (double)100.0);

    cvui::window(frame, frame.cols - 260, 180, 250, 355, "Quadric");
    cvui::text(frame, frame.cols - 250, 205, "Type");
    cvui::checkbox(frame, frame.cols - 250, 220, "Ellipsoid", &checked1);
    cvui::checkbox(frame, frame.cols - 250, 240, "Elliptic paraboloid", &checked2);
    cvui::checkbox(frame, frame.cols - 250, 260, "Hyperbolic paraboloid", &checked3);
    cvui::checkbox(frame, frame.cols - 250, 280, "Elliptic hyperboloid of one sheet", &checked4);
    cvui::checkbox(frame, frame.cols - 250, 300, "Elliptic hyperboloid of two sheets", &checked5);
    cvui::checkbox(frame, frame.cols - 250, 320, "Elliptic cone", &checked6);
    cvui::checkbox(frame, frame.cols - 250, 340, "Elliptic cylinder", &checked7);
    cvui::checkbox(frame, frame.cols - 250, 360, "Hyperbolic cylinder", &checked8);
    cvui::text(frame, frame.cols - 250, 385, "Parameters");
    cvui::text(frame, frame.cols - 252, 405, "a");
    cvui::trackbar(frame, frame.cols - 240, 395, 220, &avalue, (double)0.1, (double)10.0);
    cvui::text(frame, frame.cols - 252, 435, "b");
    cvui::trackbar(frame, frame.cols - 240, 425, 220, &bvalue, (double)0.1, (double)10.0);
    cvui::text(frame, frame.cols - 252, 465, "c");
    cvui::trackbar(frame, frame.cols - 240, 455, 220, &cvalue, (double)0.1, (double)10.0);
    cvui::text(frame, frame.cols - 252, 495, "r");
    cvui::trackbar(frame, frame.cols - 240, 485, 220, &bboxr, (double)0.5, (double)10.0);

    cvui::window(frame, frame.cols - 260, 540, 250, 85, "Other");
    cvui::checkbox(frame, frame.cols - 250, 565, "Lighting", &lighting);
    cvui::checkbox(frame, frame.cols - 250, 585, "Checkerboard", &checkerboard);
    cvui::checkbox(frame, frame.cols - 250, 605, "Contours", &contours);

    if (cvui::button(frame, frame.cols - 255, 630, "&Render")) {
      outfile_name = "render_scene.png";
      quadric->change_type(type);
      quadric->change_param_vec_(V3(avalue, bvalue, cvalue));
      quadric->change_material(get_material(lighting, checkerboard, contours));
      quadric->change_bounding_box_range(bboxr);
      scene.objects_.erase(scene.objects_.begin());
      scene.objects_.push_back(quadric);
      scene.camera_->bounding_box_range_ = bboxr;
      float lr = rotationLR * PI / 180;
      float du = (rotationDU + 90.0) * PI / 180;
      V3 v = V3((-sin(lr)) * cos(du), cos(lr) * cos (du), sin(du)).unit();
      scene.camera_->origin_coord_ = v * (-zoom);
      float s = zoom / 5.0;
      scene.camera_->plane_height_ = 2.0 * s;
      scene.camera_->plane_width_ = 3.0 * s;
      scene.lights_.at(0)->origin_coord_ = v * (-bboxr - 2.5);

      Image image(width, height);

      std::cout << "Rendering..." << std::endl;
      
      time_t start = time(0);
      scene.render(&image, width, height);
      std::cout << "Rendering finished in " << difftime(time(0), start) << " seconds." << std::endl;

      std::cout << "Writing to " << outfile_name << "." << std::endl;
      image.save(outfile_name);
      render = cv::imread(outfile_name);
    }

    if (cvui::button(frame, frame.cols - 80, 630, "&Quit")) {
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
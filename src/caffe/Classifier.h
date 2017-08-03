//
// Created by alex on 13.07.17.
//

#ifndef CAFFETEST_CLASSIFIER_H
#define CAFFETEST_CLASSIFIER_H

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>

#include "caffe/caffe.hpp"

using namespace caffe;
using std::string;

typedef std::pair<string, float> Prediction;
typedef std::pair<string, vector<vector<float>>> Heatmap;

class Classifier {
 public:
  Classifier(const string &model_file,
             const string &trained_file,
             const string &mean_file,
             const string &label_file);

  std::vector<Prediction> Classify(const cv::Mat &img, int N = 5);

  Heatmap GenerateHeatmap(const cv::Mat &img, unsigned int resolution);

 private:
  void SetMean(const string &mean_file);

  std::vector<float> Predict(const cv::Mat &img);

  void WrapInputLayer(std::vector<cv::Mat> *input_channels);

  void Preprocess(const cv::Mat &img,
                  std::vector<cv::Mat> *input_channels);

 private:
  shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
};

#endif //CAFFETEST_CLASSIFIER_H

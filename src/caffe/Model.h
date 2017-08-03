//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_MODEL_H
#define CAFFEMACHINE_MODEL_H

#include <string>
#include "../../vendors/json.h"

using std::string;
using nlohmann::json;

class Model {
 public:
  Model() {}

  Model(const string &name, unsigned int inputWidth, unsigned int inputHeight) : name_(name),
                                                                                 inputWidth_(
                                                                                     inputWidth),
                                                                                 inputHeight_(
                                                                                     inputHeight) {};

  Model(const string &name, unsigned int inputWidth, unsigned int inputHeight,
        const string &modelFile, const string &trainedFile, const string &meanFile, const string &labelFile) :
      name_(name),
      inputWidth_(
          inputWidth),
      inputHeight_(
          inputHeight),
      modelFile_(
          modelFile),
      trainedFile_(
          trainedFile),
      meanFile_(
          meanFile),
      labelFile_(
          labelFile) {}

  unsigned int GetId() const;
  void SetId(unsigned int id);

  const string &GetName() const;
  void SetName(const string &name);

  unsigned int GetInputWidth() const;
  void SetInputWidth(unsigned int inputWidth);

  unsigned int GetInputHeight() const;
  void SetInputHeight(unsigned int inputHeight);

  const string &GetModelFile() const;
  void SetModelFile(const string &modelFile);

  const string &GetTrainedFile() const;
  void SetTrainedFile(const string &trainedFile);

  const string &GetMeanFile() const;
  void SetMeanFile(const string &meanFile);

  const string &GetLabelFile() const;
  void SetLabelFile(const string &labelFile);

  static Model FromJson(json js);
  json ToJson();

 private:
  unsigned int id_ = 0;
  string name_;
  unsigned int inputWidth_;
  unsigned int inputHeight_;

  //caffe model data
  string modelFile_;
  string trainedFile_;
  string meanFile_;
  string labelFile_;
};

#endif //CAFFEMACHINE_MODEL_H

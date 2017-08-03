//
// Created by alex on 17.07.17.
//

#include "Model.h"

unsigned int Model::GetId() const {
  return id_;
}
void Model::SetId(unsigned int id) {
  Model::id_ = id;
}

const string &Model::GetName() const {
  return name_;
}
void Model::SetName(const string &name) {
  Model::name_ = name;
}

unsigned int Model::GetInputWidth() const {
  return inputWidth_;
}
void Model::SetInputWidth(unsigned int inputWidth) {
  Model::inputWidth_ = inputWidth;
}

unsigned int Model::GetInputHeight() const {
  return inputHeight_;
}
void Model::SetInputHeight(unsigned int inputHeight) {
  Model::inputHeight_ = inputHeight;
}

const string &Model::GetModelFile() const {
  return modelFile_;
}
void Model::SetModelFile(const string &modelFile) {
  Model::modelFile_ = modelFile;
}

const string &Model::GetTrainedFile() const {
  return trainedFile_;
}
void Model::SetTrainedFile(const string &trainedFile) {
  Model::trainedFile_ = trainedFile;
}

const string &Model::GetMeanFile() const {
  return meanFile_;
}
void Model::SetMeanFile(const string &meanFile) {
  Model::meanFile_ = meanFile;
}

const string &Model::GetLabelFile() const {
  return labelFile_;
}
void Model::SetLabelFile(const string &labelFile) {
  Model::labelFile_ = labelFile;
}

json Model::ToJson() {
  json j = {
      {"id", id_},
      {"name", name_},
      {"w_in", inputWidth_},
      {"h_in", inputHeight_}
  };
  return j;
}

Model Model::FromJson(json js) {
  Model model;

  //Name
  model.SetName(js.at("name").get<string>());

  //Input dimensions
  model.SetInputHeight(js.at("inputHeight").get<int>());
  model.SetInputWidth(js.at("inputWidth").get<int>());

  //Filepaths
  model.SetModelFile(js.at("modelFile").get<string>());
  model.SetTrainedFile(js.at("trainedFile").get<string>());
  model.SetMeanFile(js.at("meanFile").get<string>());
  model.SetLabelFile(js.at("labelFile").get<string>());

  return model;
}

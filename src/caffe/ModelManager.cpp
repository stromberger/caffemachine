//
// Created by alex on 17.07.17.
//

#include "ModelManager.h"

void ModelManager::AddModel(Model model) {
  model.SetId(counter_);
  models_.push_back(model);

  counter_++;
}

void ModelManager::RemoveModel(int id) {
  for (auto i = 0; i < models_.size(); i++)
    if (models_[i].GetId() == id)
      models_.erase(models_.begin() + i);
}

Model ModelManager::GetModelById(int id) {
  for (auto model : models_)
    if (model.GetId() == id)
      return model;
}

std::vector<Model> ModelManager::GetAllModels() {
  return models_;
}

bool ModelManager::ModelExists(int id) {
  for (const auto &model : models_)
    if (model.GetId() == id)
      return true;

  return false;
}

ModelManager ModelManager::FromJson(json js) {
  ModelManager manager;

  for (const auto &node : js)
    manager.AddModel(Model::FromJson(node));

  return manager;
}

std::shared_ptr<Classifier> ModelManager::GetClassifierById(unsigned int id) {
  if (classifiers_.find(id) == classifiers_.end()) {
    auto model = GetModelById(id);
    std::shared_ptr<Classifier> classifier = std::make_shared<Classifier>(model.GetModelFile(),
                          model.GetTrainedFile(),
                          model.GetMeanFile(),
                          model.GetLabelFile());
    classifiers_[id] = classifier;
  }
  return classifiers_[id];
}

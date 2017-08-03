//
// Created by alex on 17.07.17.
//

#ifndef CAFFEMACHINE_MODELMANAGER_H
#define CAFFEMACHINE_MODELMANAGER_H

#include "Model.h"
#include "Classifier.h"

class ModelManager {
 public:
  void AddModel(Model model);
  void RemoveModel(int id);
  Model GetModelById(int id);
  std::vector<Model> GetAllModels();
  bool ModelExists(int id);
  std::shared_ptr<Classifier> GetClassifierById(unsigned int id);

  static ModelManager FromJson(json js);

 private:
  std::vector<Model> models_;
  std::map<unsigned int,std::shared_ptr<Classifier>> classifiers_;
  unsigned int counter_ = 0;
};

#endif //CAFFEMACHINE_MODELMANAGER_H

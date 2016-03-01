#include "PathReader.h"

#include <QJsonArray>
#include <QJsonObject>

#include <CellarWorkbench/Misc/Log.h>

#include <CellarWorkbench/Path/CompositePath.h>
#include <CellarWorkbench/Path/CubicSplinePath.h>
#include <CellarWorkbench/Path/BasisSplinePath.h>
#include <CellarWorkbench/Path/LinearPath.h>

using namespace cellar;


template<>
double PathReader::readValue<double>(const QJsonValue& value, bool& ok)
{
    if(ok = ok && value.isDouble())
        return value.toDouble();
    else
        return nan("");
}

template<>
glm::dvec3 PathReader::readValue<glm::dvec3>(const QJsonValue& value, bool& ok)
{
    glm::dvec3 ret(nan(""));

    QJsonArray array = value.toArray();
    ok = ok &&
         array[0].isDouble() &&
         array[1].isDouble() &&
         array[2].isDouble();

    if(ok)
    {
        ret.x = array[0].toDouble();
        ret.y = array[1].toDouble();
        ret.z = array[2].toDouble();
    }

    return ret;
}

PathReader::PathReader()
{

}

PathReader::~PathReader()
{

}

bool PathReader::read(const QJsonDocument& jsonDoc)
{
    QJsonArray pathsArray = jsonDoc.array();

    for(QJsonValue compVal : pathsArray)
    {
        QJsonObject compObj = compVal.toObject();
        if(compObj["Type"] != "CompositePath")
        {
            getLog().postMessage(new Message('E', false,
                "Top path must CompositePath while this path is "
                    + compObj["Type"].toString().toStdString(), "PathReader"));
            return false;
        }

        QString pathData = compObj["Data"].toString();
        cellar::CompositePath<double>* doubleComposite = nullptr;
        cellar::CompositePath<glm::dvec3>* dvec3Composite = nullptr;
        if(pathData == "double")
        {
            doubleComposite = new cellar::CompositePath<double>();
        }
        else if(pathData == "dvec3")
        {
            dvec3Composite = new cellar::CompositePath<glm::dvec3>();
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Paths are only specialized for 'doubes' or 'dvec3s'." \
                "This one is a " + pathData.toStdString(), "PathReader"));
            return false;
        }

        for(QJsonValue pathVal : compObj["Paths"].toArray())
        {
            bool ok = true;

            QJsonObject pathObj = pathVal.toObject();
            double duration = pathObj["Duration"].toDouble();
            QJsonArray ctrlPts = pathObj["CtrlPts"].toArray();
            if(pathObj["Type"] == "LinearPath")
            {
                if(doubleComposite)
                {
                    double begin = readValue<double>(ctrlPts[0], ok);
                    double end = readValue<double>(ctrlPts[1], ok);
                    doubleComposite->addPath(
                        std::make_shared<cellar::LinearPath<double>>(
                            duration, begin, end));
                }
                else
                {
                    glm::dvec3 begin = readValue<glm::dvec3>(ctrlPts[0], ok);
                    glm::dvec3 end = readValue<glm::dvec3>(ctrlPts[1], ok);
                    dvec3Composite->addPath(
                        std::make_shared<cellar::LinearPath<glm::dvec3>>(
                            duration, begin, end));
                }
            }
            else if(pathObj["Type"] == "CubicSplinePath")
            {
                if(doubleComposite)
                {
                    std::vector<double> ctrlPts;
                    for(QJsonValue cp : pathObj["CtrlPts"].toArray())
                        ctrlPts.push_back(readValue<double>(cp, ok));

                    doubleComposite->addPath(
                        std::make_shared<cellar::CubicSplinePath<double>>(
                            duration, ctrlPts));
                }
                else
                {
                    std::vector<glm::dvec3> ctrlPts;
                    for(QJsonValue cp : pathObj["CtrlPts"].toArray())
                        ctrlPts.push_back(readValue<glm::dvec3>(cp, ok));

                    dvec3Composite->addPath(
                        std::make_shared<cellar::CubicSplinePath<glm::dvec3>>(
                            duration, ctrlPts));
                }
            }
            else if(pathObj["Type"] == "BasisSplinePath")
            {
                if(doubleComposite)
                {
                    std::vector<double> ctrlPts;
                    for(QJsonValue cp : pathObj["CtrlPts"].toArray())
                        ctrlPts.push_back(readValue<double>(cp, ok));

                    doubleComposite->addPath(
                        std::make_shared<cellar::BasisSplinePath<double>>(
                            duration, ctrlPts));
                }
                else
                {
                    std::vector<glm::dvec3> ctrlPts;
                    for(QJsonValue cp : pathObj["CtrlPts"].toArray())
                        ctrlPts.push_back(readValue<glm::dvec3>(cp, ok));

                    dvec3Composite->addPath(
                        std::make_shared<cellar::BasisSplinePath<glm::dvec3>>(
                            duration, ctrlPts));
                }
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown path " + pathObj["Type"].toString().toStdString(),
                    "PathReader"));
                return false;
            }

            if(!ok)
            {
                getLog().postMessage(new Message('E', false,
                    "One of the control points couldn't be parsed",
                    "PathReader"));
                return false;
            }
        }

        if(doubleComposite)
        {
            _doublePaths[compObj["Name"].toString().toStdString()] =
                std::shared_ptr<cellar::AbstractPath<double>>(doubleComposite);
        }
        else
        {
            _dvec3Paths[compObj["Name"].toString().toStdString()] =
                std::shared_ptr<cellar::AbstractPath<glm::dvec3>>(dvec3Composite);
        }
    }

    return true;
}

std::shared_ptr<cellar::AbstractPath<double>> PathReader::doublePath(const std::string& name)
{
    return _doublePaths[name];
}

std::shared_ptr<cellar::AbstractPath<glm::dvec3>> PathReader::dvec3Path(const std::string& name)
{
    return _dvec3Paths[name];
}

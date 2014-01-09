/*
 * Copyright (C)2011  Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: Andrea Del Prete
 * email: andrea.delprete@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#include "wbiIcub/wholeBodyInterfaceIcub.h"
#include <iCub/skinDynLib/common.h>
#include <yarp/os/Time.h>
#include <string>


using namespace std;
using namespace wbi;
using namespace wbiIcub;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace iCub::skinDynLib;
using namespace iCub::ctrl;

//\todo TODO make it a proper parameter
#define ESTIMATOR_PERIOD 10

// iterate over all body parts
#define FOR_ALL_BODY_PARTS(itBp)            FOR_ALL_BODY_PARTS_OF(itBp, jointIdList)
// iterate over all joints of all body parts
#define FOR_ALL(itBp, itJ)                  FOR_ALL_OF(itBp, itJ, jointIdList)


// *********************************************************************************************************************
// *********************************************************************************************************************
//                                          ICUB WHOLE BODY STATES
// *********************************************************************************************************************
// *********************************************************************************************************************
icubWholeBodyStatesLocal::icubWholeBodyStatesLocal(const char* _name, const char* _robotName, double estimationTimeWindow)
{
    sensors = new icubWholeBodySensors(_name, _robotName);              // sensor interface
    skin_contacts_port = new yarp::os::BufferedPort<iCub::skinDynLib::skinContactList>;
    skin_contacts_port->open(string("/"+_name+"/skin_contacts:i").c_str());
    estimator = new icubWholeBodyDynamicsEstimator(ESTIMATOR_PERIOD, sensors, skin_contacts_port);  // estimation thread
}

bool icubWholeBodyStatesLocal::init()
{
    bool ok = sensors->init();              // initialize sensor interface
    return ok ? estimator->start() : false; // start estimation thread
}

bool icubWholeBodyStatesLocal::close()
{
    if(estimator)   estimator->stop();  // stop estimator BEFORE closing sensor interface
    bool ok = (sensors ? sensors->close() : true);
    if(sensors)     delete sensors;
    if(estimator)   delete estimator;
    return ok;
}

bool icubWholeBodyStatesLocal::addEstimate(const EstimateType et, const LocalId &sid)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_VEL:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_ACC:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_TORQUE:             return lockAndAddSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return lockAndAddSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_POS:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_VEL:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_ACC:                return lockAndAddSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_TORQUE:             return lockAndAddSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return lockAndAddSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_PWM:                return lockAndAddSensor(SENSOR_PWM, sid);
    case ESTIMATE_IMU:                      return lockAndAddSensor(SENSOR_IMU, sid);
    case ESTIMATE_FORCE_TORQUE:             return lockAndAddSensor(SENSOR_FORCE_TORQUE, sid);
    default: break;
    }
    return false;
}
        
int icubWholeBodyStatesLocal::addEstimates(const EstimateType et, const LocalIdList &sids)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_JOINT_VEL:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_JOINT_ACC:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_JOINT_TORQUE:             return lockAndAddSensors(SENSOR_TORQUE, sids);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return lockAndAddSensors(SENSOR_TORQUE, sids);
    case ESTIMATE_MOTOR_POS:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_MOTOR_VEL:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_MOTOR_ACC:                return lockAndAddSensors(SENSOR_ENCODER, sids);
    case ESTIMATE_MOTOR_TORQUE:             return lockAndAddSensors(SENSOR_TORQUE, sids);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return lockAndAddSensors(SENSOR_TORQUE, sids);
    case ESTIMATE_MOTOR_PWM:                return lockAndAddSensors(SENSOR_PWM, sids);
    case ESTIMATE_IMU:                      return lockAndAddSensors(SENSOR_IMU, sids);
    case ESTIMATE_FORCE_TORQUE:             return lockAndAddSensors(SENSOR_FORCE_TORQUE, sids);
    default: break;
    }
    return false;
}

bool icubWholeBodyStatesLocal::removeEstimate(const EstimateType et, const LocalId &sid)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_VEL:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_ACC:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_JOINT_TORQUE:             return lockAndRemoveSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return lockAndRemoveSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_POS:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_VEL:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_ACC:                return lockAndRemoveSensor(SENSOR_ENCODER, sid);
    case ESTIMATE_MOTOR_TORQUE:             return lockAndRemoveSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return lockAndRemoveSensor(SENSOR_TORQUE, sid);
    case ESTIMATE_MOTOR_PWM:                return lockAndRemoveSensor(SENSOR_PWM, sid);
    case ESTIMATE_IMU:                      return lockAndRemoveSensor(SENSOR_IMU, sid);
    case ESTIMATE_FORCE_TORQUE:             return lockAndRemoveSensor(SENSOR_FORCE_TORQUE, sid);
    default: break;
    }
    return false;
}
        
const LocalIdList& icubWholeBodyStatesLocal::getEstimateList(const EstimateType et)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_JOINT_VEL:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_JOINT_ACC:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_JOINT_TORQUE:             return sensors->getSensorList(SENSOR_TORQUE);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return sensors->getSensorList(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_POS:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_VEL:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_ACC:                return sensors->getSensorList(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_TORQUE:             return sensors->getSensorList(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return sensors->getSensorList(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_PWM:                return sensors->getSensorList(SENSOR_PWM);
    case ESTIMATE_IMU:                      return sensors->getSensorList(SENSOR_IMU);
    case ESTIMATE_FORCE_TORQUE:             return sensors->getSensorList(SENSOR_FORCE_TORQUE);
    default: break;
    }
    return emptyList;
}
        
int icubWholeBodyStatesLocal::getEstimateNumber(const EstimateType et)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_JOINT_VEL:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_JOINT_ACC:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_JOINT_TORQUE:             return sensors->getSensorNumber(SENSOR_TORQUE);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return sensors->getSensorNumber(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_POS:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_VEL:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_ACC:                return sensors->getSensorNumber(SENSOR_ENCODER);
    case ESTIMATE_MOTOR_TORQUE:             return sensors->getSensorNumber(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return sensors->getSensorNumber(SENSOR_TORQUE);
    case ESTIMATE_MOTOR_PWM:                return sensors->getSensorNumber(SENSOR_PWM);
    case ESTIMATE_IMU:                      return sensors->getSensorNumber(SENSOR_IMU);
    case ESTIMATE_FORCE_TORQUE:             return sensors->getSensorNumber(SENSOR_FORCE_TORQUE);
    default: break;
    }
    return 0;
}

bool icubWholeBodyStatesLocal::getEstimate(const EstimateType et, const LocalId &sid, double *data, double time, bool blocking)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_ENCODER).localToGlobalId(sid), estimator->estimates.lastQ, data);
    case ESTIMATE_JOINT_VEL:
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_ENCODER).localToGlobalId(sid), estimator->estimates.lastDq, data);
    case ESTIMATE_JOINT_ACC:
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_ENCODER).localToGlobalId(sid), estimator->estimates.lastD2q, data);
    case ESTIMATE_JOINT_TORQUE:     
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_TORQUE).localToGlobalId(sid), estimator->estimates.lastTauJ, data);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_TORQUE).localToGlobalId(sid), estimator->estimates.lastDtauJ, data);
    case ESTIMATE_MOTOR_POS:        
        return false;
    case ESTIMATE_MOTOR_VEL:        
        return getMotorVel(sid, data, time, blocking);
    case ESTIMATE_MOTOR_ACC:        
        return false;
    case ESTIMATE_MOTOR_TORQUE:     
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_TORQUE).localToGlobalId(sid), estimator->estimates.lastTauM, data);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:     
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_TORQUE).localToGlobalId(sid), estimator->estimates.lastDtauM, data);
    case ESTIMATE_MOTOR_PWM:        
        return lockAndReadSensor(SENSOR_PWM, sid, data, time, blocking);
    case ESTIMATE_IMU:              
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_IMU).localToGlobalId(sid), estimator->estimates.lastIMUs, data);
    case ESTIMATE_FORCE_TORQUE:     
        return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_FORCE_TORQUE).localToGlobalId(sid), estimator->estimates.lastForceTorques, data);
    default: break;
    }
    return false;
}

bool icubWholeBodyStatesLocal::getEstimates(const EstimateType et, double *data, double time, bool blocking)
{
    switch(et)
    {
    case ESTIMATE_JOINT_POS:                return estimator->lockAndCopyVector(estimator->estimates.lastQ, data);
    case ESTIMATE_JOINT_VEL:                return estimator->lockAndCopyVector(estimator->estimates.lastDq, data);
    case ESTIMATE_JOINT_ACC:                return estimator->lockAndCopyVector(estimator->estimates.lastD2q, data);
    case ESTIMATE_JOINT_TORQUE:             return estimator->lockAndCopyVector(estimator->estimates.lastTauJ, data);
    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:  return estimator->lockAndCopyVector(estimator->estimates.lastDtauJ, data);
    case ESTIMATE_MOTOR_POS:                return false;
    case ESTIMATE_MOTOR_VEL:                return getMotorVel(data, time, blocking);
    case ESTIMATE_MOTOR_ACC:                return false;
    case ESTIMATE_MOTOR_TORQUE:             return estimator->lockAndCopyVector(estimator->estimates.lastTauM, data);
    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:  return estimator->lockAndCopyVector(estimator->estimates.lastDtauM, data);
    case ESTIMATE_MOTOR_PWM:                return lockAndReadSensors(SENSOR_PWM, data, time, blocking);
    case ESTIMATE_IMU:                      return estimator->lockAndCopyVectorFromVector(estimator->estimates.lastIMUs, data);
    case ESTIMATE_FORCE_TORQUE:             return estimator->lockAndCopyVectorFromVector(estimator->estimates.lastForceTorques, data);
    default: break;
    }
    return false;
}

bool icubWholeBodyStatesLocal::setEstimationParameter(const EstimateType et, const EstimationParameter ep, const void *value)
{
    return estimator->lockAndSetEstimationParameter(et, ep, value);
}

// *********************************************************************************************************************
// *********************************************************************************************************************
//                                          PRIVATE METHODS
// *********************************************************************************************************************
// *********************************************************************************************************************

bool icubWholeBodyStatesLocal::getMotorVel(double *data, double time, bool blocking)
{
    bool res = estimator->lockAndCopyVector(estimator->estimates.lastDq, data);    ///< read joint vel
    if(!res) return false;
    LocalIdList idList = lockAndGetSensorList(SENSOR_ENCODER);
    int i=0;
    FOR_ALL_OF(itBp, itJ, idList)   ///< manage coupled joints
    {
        if(itBp->first == LEFT_ARM && *itJ==0)          // left arm shoulder
            data[i] = data[i];
        else if(itBp->first == LEFT_ARM && *itJ==1)     // left arm shoulder
            data[i] = data[i];
        else if(itBp->first == LEFT_ARM && *itJ==2)     // left arm shoulder
            data[i] = data[i];
        else if(itBp->first == RIGHT_ARM && *itJ==0)    // right arm shoulder
            data[i] = data[i];
        else  if(itBp->first == RIGHT_ARM && *itJ==1)   // right arm shoulder
            data[i] = data[i];
        else if(itBp->first == RIGHT_ARM && *itJ==2)    // right arm shoulder
            data[i] = data[i];
        else if(itBp->first == TORSO && *itJ==1)        // torso
            data[i] = data[i];
        else if(itBp->first == TORSO && *itJ==2)        // torso
            data[i] = data[i];
        i++;
    }
    return true;
}

bool icubWholeBodyStatesLocal::getMotorVel(const LocalId &lid, double *data, double time, bool blocking)
{
    ///< read joint vel
    return estimator->lockAndCopyVectorElement(sensors->getSensorList(SENSOR_ENCODER).localToGlobalId(lid), estimator->estimates.lastDq, data);
}

bool icubWholeBodyStatesLocal::lockAndReadSensors(const SensorType st, double *data, double time, bool blocking)
{
    estimator->mutex.wait();
    bool res = sensors->readSensors(st, data, 0, blocking); 
    estimator->mutex.post();
    return res;
}

bool icubWholeBodyStatesLocal::lockAndReadSensor(const SensorType st, const LocalId sid, double *data, double time, bool blocking)
{
    estimator->mutex.wait();
    bool res = sensors->readSensor(st, sid, data, 0, blocking); 
    estimator->mutex.post();
    return res;
}

bool icubWholeBodyStatesLocal::lockAndAddSensor(const SensorType st, const LocalId &sid)
{
    estimator->mutex.wait();
    bool res = sensors->addSensor(st, sid); 
    estimator->mutex.post();
    return res;
}

int icubWholeBodyStatesLocal::lockAndAddSensors(const SensorType st, const LocalIdList &sids)
{
    estimator->mutex.wait();
    int res = sensors->addSensors(st, sids);
    estimator->mutex.post();
    return res;
}

bool icubWholeBodyStatesLocal::lockAndRemoveSensor(const SensorType st, const LocalId &sid)
{
    estimator->mutex.wait();
    bool res = sensors->removeSensor(st, sid);
    estimator->mutex.post();
    return res;
}

LocalIdList icubWholeBodyStatesLocal::lockAndGetSensorList(const SensorType st)
{
    estimator->mutex.wait();
    LocalIdList res = sensors->getSensorList(st); 
    estimator->mutex.post();
    return res;
}

int icubWholeBodyStatesLocal::lockAndGetSensorNumber(const SensorType st)
{
    estimator->mutex.wait();
    int res = sensors->getSensorNumber(st);
    estimator->mutex.post();
    return res;
}

// *********************************************************************************************************************
// *********************************************************************************************************************
//                                          ICUB WHOLE BODY DYNAMICS ESTIMATOR
// *********************************************************************************************************************
// *********************************************************************************************************************
icubWholeBodyDynamicsEstimator::icubWholeBodyDynamicsEstimator(int _period, icubWholeBodySensors *_sensors, yarp::os::BufferedPort<iCub::skinDynLib::skinContactList> * _port_skin_contacts)
: RateThread(_period), sensors(_sensors), port_skin_contacts(_port_skin_contacts), dqFilt(0), d2qFilt(0)
{
    resizeAll(sensors->getSensorNumber(SENSOR_ENCODER));
    resizeFTs(sensors->getSensorNumber(SENSOR_FORCE_TORQUE));
    resizeIMUs(sensors->getSensorNumber(SENSOR_IMU));
    
    ///< Window lengths of adaptive window filters
    dqFiltWL            = 16;
    d2qFiltWL           = 25;
    dTauJFiltWL         = 30;
    dTauMFiltWL         = 30;
    ///< Threshold of adaptive window filters
    dqFiltTh            = 1.0;      
    d2qFiltTh           = 1.0;
    dTauJFiltTh         = 0.2;
    dTauMFiltTh         = 0.2;
    ///< Cut frequencies
    tauJCutFrequency    =   3.0;
    tauMCutFrequency    =   3.0;
    pwmCutFrequency     =   3.0;
}

bool icubWholeBodyDynamicsEstimator::threadInit()
{
    resizeAll(sensors->getSensorNumber(SENSOR_ENCODER));
    resizeFTs(sensors->getSensorNumber(SENSOR_FORCE_TORQUE));
    resizeIMUs(sensors->getSensorNumber(SENSOR_IMU));

    ///< create derivative filters
    dqFilt = new AWLinEstimator(dqFiltWL, dqFiltTh);
    d2qFilt = new AWQuadEstimator(d2qFiltWL, d2qFiltTh);
    dTauJFilt = new AWLinEstimator(dTauJFiltWL, dTauJFiltTh);
    dTauMFilt = new AWLinEstimator(dTauMFiltWL, dTauMFiltTh);
    ///< read sensors
    bool ok = sensors->readSensors(SENSOR_ENCODER, estimates.lastQ.data(), qStamps.data(), true);
    ok = ok && sensors->readSensors(SENSOR_TORQUE, estimates.lastTauJ.data(), tauJStamps.data(), true);
    ok = ok && sensors->readSensors(SENSOR_PWM, estimates.lastPwm.data(), pwmStamps.data(), true);
    
    ///< create low pass filters
    tauJFilt    = new FirstOrderLowPassFilter(tauJCutFrequency, getRate()*1e-3, estimates.lastTauJ);
    tauMFilt    = new FirstOrderLowPassFilter(tauMCutFrequency, getRate()*1e-3, estimates.lastTauJ);
    pwmFilt     = new FirstOrderLowPassFilter(pwmCutFrequency, getRate()*1e-3, estimates.lastPwm);
    return ok;
}

void icubWholeBodyDynamicsEstimator::run()
{
    ///< \todo improve robustness: what if a sensor dies or stop working? interface should warn the user
    mutex.wait();
    {
        resizeAll(sensors->getSensorNumber(SENSOR_ENCODER));
        resizeFTs(sensors->getSensorNumber(SENSOR_FORCE_TORQUE));
        resizeIMUs(sensors->getSensorNumber(SENSOR_IMU));
        
        ///< Read encoders
        if(sensors->readSensors(SENSOR_ENCODER, q.data(), qStamps.data(), false))
        {
            estimates.lastQ = q;
            AWPolyElement el;
            el.data = q;
            el.time = qStamps[0];
            estimates.lastDq = dqFilt->estimate(el);
            estimates.lastD2q = d2qFilt->estimate(el);
        }
        
        ///< Read force/torque sensors
        ///< \todo TODO buffer value of available_ft_sensors to avoid memory allocation (?)
        LocalIdList available_ft_sensors = sensors->getSensorList(SENSOR_FORCE_TORQUE);
        int i=0;
        for(LocalIdList::const_iterator id = available_ft_sensors.begin(); id != available_ft_sensors.end(); id++ )
        {
            sensors->readSensor(SENSOR_FORCE_TORQUE, *id, forcetorques[i].data(), &(forcetorquesStamps[i]),false );
            i++;
        }
        
        ///< Read IMU
        ///< \todo TODO buffer value of available_imu_sensors to avoid memory allocation (?)
        ///< \todo TODO add filters for imu values -> 
        LocalIdList available_imu_sensors = sensors->getSensorList(SENSOR_IMU);
        int i=0;
        for(LocalIdList::const_iterator id = available_imu_sensors.begin(); id != available_imu_sensors.end(); id++ )
        {
            sensors->readSensor(SENSOR_IMU, *id, IMUs[i].data(), &(IMUStamps[i]),false );
            i++;
        }
        
        ///< Read skin contacts
        readSkinContacts();
        
        ///< Estimate joint torque sensors from force/torque measurements
        estimateExternalForcesAndJointTorques();
        
        ///< Filter obtained joint torque measures
        {
            // @todo Convert joint torques into motor torques
            AWPolyElement el;
            el.time = yarp::os::Time::now();

            estimates.lastTauJ = tauJFilt->filt(tauJ);  ///< low pass filter
            estimates.lastTauM = tauMFilt->filt(tauJ);  ///< low pass filter

            el.data = tauJ;
            estimates.lastDtauJ = dTauJFilt->estimate(el);  ///< derivative filter

            //el.data = tauM;
            estimates.lastDtauM = dTauMFilt->estimate(el);  ///< derivative filter
        }

        ///< Read motor pwm
        sensors->readSensors(SENSOR_PWM, pwm.data(), pwmStamps.data(), false);
        estimates.lastPwm = pwmFilt->filt(pwm);     ///< low pass filter
    }
    mutex.post();
    
    return;
}

void icubWholeBodyDynamicsEstimator::readSkinContacts()
{
    skinContactList *scl = port_skin_contacts->read(false);
    if(scl)
    {
        //< \todo TODO check for envelope
        last_reading_skin_contact_list_Stamp = Time::now();
        if(scl->empty())   // if no skin contacts => leave the old contacts but reset pressure and contact list
        {
            for(skinContactList::iterator it=skinContacts.begin(); it!=skinContacts.end(); it++)
            {
                it->setPressure(0.0);
                it->setActiveTaxels(0);
            }
            return;
        }
        
        //Probably source of crazy inefficiencies, here just to reach a working state as soon as possible \todo TODO
        map<BodyPart, skinContactList> contactsPerBp = scl->splitPerBodyPart();
        // if there are more than 1 contact and less than 10 taxels are active then suppose zero moment
        for(map<BodyPart,skinContactList>::iterator it=contactsPerBp.begin(); it!=contactsPerBp.end(); it++)
            if(it->second.size()>1)
                for(skinContactList::iterator c=it->second.begin(); c!=it->second.end(); c++)
                    if(c->getActiveTaxels()<10)
                        c->fixMoment();
        
        //TODO \todo add other parts
        skinContacts = contactsPerBp[LEFT_ARM];
        skinContacts.insert(skinContacts.end(), contactsPerBp[RIGHT_ARM].begin(), contactsPerBp[RIGHT_ARM].end());
    
    }
    else if(Time::now()-last_reading_skin_contact_list_Stamp>SKIN_EVENTS_TIMEOUT && last_reading_skin_contact_list_Stamp!=0.0)
    {
        // if time is up, use default contact points \todo TODO
        assert(false);
        skinContacts.clear();
    }
}


void icubWholeBodyDynamicsEstimator::estimateExternalForcesAndJointTorques()
{
    //Assume that only a IMU is available
    icub_model->setInertialMeasure();
    icub_model->setAng();
    icub_model->setDAng();
    icub_model->setD2Ang();
    icub_model->setSensorMeasurement();
    icub_model->setContacts();
    
    icub_model->kinematicRNEA();
    icub_model->estimateContactForces();
    icub_model->dynamicRNEA();
    
    icub_model->getContacts();
    icub_model->getTorques();
    icub_model->getBaseAcc();
}

void icubWholeBodyDynamicsEstimator::threadRelease()
{
    // this causes a memory access violation (to investigate)
    //if(dqFilt!=NULL)    delete dqFilt;
    //if(d2qFilt!=NULL)   delete d2qFilt;
    return;
}

void icubWholeBodyDynamicsEstimator::lockAndResizeAll(int n)
{
    mutex.wait();
    resizeAll(n);
    mutex.post();
}

void icubWholeBodyDynamicsEstimator::resizeAll(int n)
{
    q.resize(n);
    qStamps.resize(n);
    tauJ.resize(n);
    tauJStamps.resize(n);
    pwm.resize(n);
    pwmStamps.resize(n);
    estimates.lastQ.resize(n);
    estimates.lastDq.resize(n);
    estimates.lastD2q.resize(n);
    estimates.lastTauJ.resize(n);
    estimates.lastTauM.resize(n);
    estimates.lastDtauJ.resize(n);
    estimates.lastDtauM.resize(n);
    estimates.lastPwm.resize(n);
}

void icubWholeBodyDynamicsEstimator::lockAndResizeFTs(int n)
{
    mutex.wait();
    resizeFTs(n);
    mutex.post();
}

void icubWholeBodyDynamicsEstimator::resizeFTs(int n)
{
    forcetorques.resize(n,Vector(6,0.0));
    forcetorquesStamps.resize(n);
    estimates.lastForceTorques.resize(n,Vector(6,0.0));
}

void icubWholeBodyDynamicsEstimator::lockAndResizeIMUs(int n)
{
    mutex.wait();
    resizeIMUs(n);
    mutex.post();
}

void icubWholeBodyDynamicsEstimator::resizeIMUs(int n)
{
    IMUs.resize(n,Vector(12,0.0));
    IMUStamps.resize(n);
    estimates.lastIMUs.resize(n,Vector(12,0.0));
}

bool icubWholeBodyDynamicsEstimator::lockAndCopyVector(const Vector &src, double *dest)
{
    if(dest==0)
        return false;
    mutex.wait();
    memcpy(dest, src.data(), sizeof(double)*src.size());
    mutex.post();
    return true;
}

bool icubWholeBodyDynamicsEstimator::lockAndCopyVectorElement(int index, const Vector &src, double *dest)
{
    mutex.wait();
    dest[0] = src[index];
    mutex.post();
    return true;
}

bool icubWholeBodyDynamicsEstimator::lockAndCopyElementVectorFromVector(int index, const std::vector<Vector> &src, double *dest)
{
    if(dest==0)
        return false;
    mutex.wait();
    memcpy(dest,src[index].data(),sizeof(double)*src[index].size());
    mutex.post();
    return true;
}

bool icubWholeBodyDynamicsEstimator::lockAndCopyVectorOfVector(const std::vector<Vector> &src, double *dest)
{
    if(dest==0)
        return false;
    mutex.wait();
    for(int i=0, offset = 0; i < src.size(); i++) {
        memcpy(dest+offset,src[i].data(),sizeof(double)*src[i].size());
        offset += src[i].size();
    }
    mutex.post();
    return true;
}

bool icubWholeBodyDynamicsEstimator::lockAndSetEstimationParameter(const EstimateType et, const EstimationParameter ep, const void *value)
{
    bool res;
    mutex.wait();
    switch(et)
    {
    case ESTIMATE_JOINT_VEL:
    case ESTIMATE_MOTOR_VEL:
        if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_MAX_SIZE) 
            res = setVelFiltParams(((int*)value)[0], dqFiltTh);
        else if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_THRESHOLD)
            res = setVelFiltParams(dqFiltWL, ((double*)value)[0]);
        break;

    case ESTIMATE_JOINT_ACC:
    case ESTIMATE_MOTOR_ACC:
        if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_MAX_SIZE) 
            res = setAccFiltParams(((int*)value)[0], d2qFiltTh);
        else if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_THRESHOLD)
            res = setAccFiltParams(d2qFiltWL, ((double*)value)[0]);
        break;

    case ESTIMATE_JOINT_TORQUE:
        if(ep==ESTIMATION_PARAM_LOW_PASS_FILTER_CUT_FREQ)
            res = setTauJCutFrequency(((double*)value)[0]);
        break;

    case ESTIMATE_JOINT_TORQUE_DERIVATIVE:
        if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_MAX_SIZE) 
            res = setDtauJFiltParams(((int*)value)[0], dTauJFiltTh);
        else if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_THRESHOLD)
            res = setDtauJFiltParams(dTauJFiltWL, ((double*)value)[0]);
        break;

    case ESTIMATE_MOTOR_TORQUE:
        if(ep==ESTIMATION_PARAM_LOW_PASS_FILTER_CUT_FREQ)
            res = setTauMCutFrequency(((double*)value)[0]);
        break;

    case ESTIMATE_MOTOR_TORQUE_DERIVATIVE:
        if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_MAX_SIZE) 
            res = setDtauMFiltParams(((int*)value)[0], dTauMFiltTh);
        else if(ep==ESTIMATION_PARAM_ADAPTIVE_WINDOW_THRESHOLD)
            res = setDtauMFiltParams(dTauMFiltWL, ((double*)value)[0]);
        break;
    
    case ESTIMATE_MOTOR_PWM:
        if(ep==ESTIMATION_PARAM_LOW_PASS_FILTER_CUT_FREQ)
            res = setPwmCutFrequency(((double*)value)[0]);
        break;

    case ESTIMATE_IMU:
    case ESTIMATE_FORCE_TORQUE:
    case ESTIMATE_JOINT_POS:
    case ESTIMATE_MOTOR_POS:    
    default: break;
    }
    mutex.post();
    return res;
}

bool icubWholeBodyDynamicsEstimator::setVelFiltParams(int windowLength, double threshold)
{
    if(windowLength<1 || threshold<=0.0)
        return false;
    dqFiltWL = windowLength;
    dqFiltTh = threshold;
    if(dqFilt!=NULL)
    {
        AWPolyList list = dqFilt->getList();
        dqFilt = new AWLinEstimator(dqFiltWL, dqFiltTh);
        for(AWPolyList::iterator it=list.begin(); it!=list.end(); it++)
            dqFilt->feedData(*it);
    }
    return true;
}

bool icubWholeBodyDynamicsEstimator::setAccFiltParams(int windowLength, double threshold)
{
    if(windowLength<1 || threshold<=0.0)
        return false;
    d2qFiltWL = windowLength;
    d2qFiltTh = threshold;
    if(d2qFilt!=NULL)
    {
        AWPolyList list = d2qFilt->getList();
        d2qFilt = new AWQuadEstimator(d2qFiltWL, d2qFiltTh);
        for(AWPolyList::iterator it=list.begin(); it!=list.end(); it++)
            d2qFilt->feedData(*it);
    }
    return true;
}

bool icubWholeBodyDynamicsEstimator::setDtauJFiltParams(int windowLength, double threshold)
{
    if(windowLength<1 || threshold<=0.0)
        return false;
    dTauJFiltWL = windowLength;
    dTauJFiltTh = threshold;
    if(dTauJFilt!=NULL)
    {
        AWPolyList list = dTauJFilt->getList();
        dTauJFilt = new AWLinEstimator(windowLength, threshold);
        for(AWPolyList::iterator it=list.begin(); it!=list.end(); it++)
            dTauJFilt->feedData(*it);
    }
    return true;
}

bool icubWholeBodyDynamicsEstimator::setDtauMFiltParams(int windowLength, double threshold)
{
    if(windowLength<1 || threshold<=0.0)
        return false;
    dTauMFiltWL = windowLength;
    dTauMFiltTh = threshold;
    if(dTauMFilt!=NULL)
    {
        AWPolyList list = dTauMFilt->getList();
        dTauMFilt = new AWLinEstimator(windowLength, threshold);
        for(AWPolyList::iterator it=list.begin(); it!=list.end(); it++)
            dTauMFilt->feedData(*it);
    }
    return true;
}

bool icubWholeBodyDynamicsEstimator::setTauJCutFrequency(double fc)
{
    return tauJFilt->setCutFrequency(fc);
}

bool icubWholeBodyDynamicsEstimator::setTauMCutFrequency(double fc)
{
    return tauMFilt->setCutFrequency(fc);
}

bool icubWholeBodyDynamicsEstimator::setPwmCutFrequency(double fc)
{
    return pwmFilt->setCutFrequency(fc);
}
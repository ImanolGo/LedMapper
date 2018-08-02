/*
 *  ResourceManager.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */

#include "../Main/AppManager.h"
#include "../Main/SettingsManager.h"

#include "ResourceManager.h"

const int ResourceManager::DEFAULT_IMAGE_SIZE = 256;


ResourceManager::ResourceManager() : Manager()
{
    //Intentionally left empty
}

ResourceManager::~ResourceManager()
{
    ofLogNotice() <<"ResourceManager::destructor" ;
}


void ResourceManager::setup()
{
    if(m_initialized)
		return;

    ofLogNotice() <<"ResourceManager::initialized" ;

	Manager::setup();

    this->createDefaultResource();
    this->loadResources();
}

void ResourceManager::loadResources()
{
    this->loadTextures();
}

void ResourceManager::loadTextures()
{
    ResourcesPathMap texturePathMap = AppManager::getInstance().getSettingsManager().getTextureResourcesPath();

    for(ResourcesPathMap::iterator it = texturePathMap.begin(); it!= texturePathMap.end(); it++)
    {
        string textureName = it->first;
        string texturePath = it->second;

        ofPtr<ofTexture> texture = ofPtr<ofTexture>(new ofTexture());

        if(ofLoadImage(*texture,texturePath)){
            m_textures[textureName] = texture;
            ofLogNotice() <<"ResourceManager::loadTextures-> allocated texture " << textureName ;

        }
        else{
            ofLogNotice() <<"ResourceManager::loadTextures-> unable to load texture " << textureName
            << " from path " << texturePath ;
        }
    }
}


ofPtr<ofTexture> ResourceManager::getTexture(const string& name) const
{
    if(this->containsTexture(name)) {
		return m_textures.at(name);
	}

	return m_defaultTexture;
}
void ResourceManager::createDefaultResource()
{
    int resourceSize = 256;
    m_defaultTexture = ofPtr<ofTexture>(new ofTexture());
    m_defaultTexture->allocate(DEFAULT_IMAGE_SIZE,DEFAULT_IMAGE_SIZE,GL_RGB);
}

bool ResourceManager::containsTexture(const string& name) const
{
	if(m_textures.find(name) == m_textures.end()) {
        ofLogNotice() <<"ResourceManager::containsTexture-> no resource with name " << name ;
		return false; // no entries for the specified name
	}

	return true;
}








/*
 * tttSeedContainer.h
 *
 *  Created on: Jun 15, 2016
 *      Author: morgan
 */

#ifndef TTTSEEDCONTAINER_H_
#define TTTSEEDCONTAINER_H_

namespace ttt{

class SeedContainer{
public:
	typedef SeedContainer Self;
	typedef std::shared_ptr<Self> Pointer;

	typedef itk::Index<3> SeedType;

	static Pointer New(){
		return std::make_shared<Self>();
	}

	void AddSeed(const SeedType & seed){
		m_Seeds.push_back(seed);
	}

	SeedType GetSeed(unsigned index){
		return m_Seeds[index];
	}

	unsigned GetNumberOfSeeds(){
		return m_Seeds.size();
	}

public:

	SeedContainer(){

	}
	virtual ~SeedContainer(){

	}

private:
	std::vector<SeedType> m_Seeds;
};



}

#endif /* TTTSEEDCONTAINER_H_ */

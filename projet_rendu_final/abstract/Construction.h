#pragma once
#include <string>
#include <vector>
class Construction {
protected:
	int id_tuile_mere;
public:
	virtual std::string get_type_construction() const = 0;
	virtual void afficher_construction() const = 0;
	int get_id_mere() const { return id_tuile_mere;}
	Construction(const Construction&) = delete;
	Construction& operator=(const Construction&) = delete;
	Construction(int id) : id_tuile_mere(id) {}

	virtual ~Construction() = default;
};

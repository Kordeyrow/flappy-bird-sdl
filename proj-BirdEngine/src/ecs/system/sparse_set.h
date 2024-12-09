#pragma once
#include <vector>

template<typename Data>
class SparseSet {
public:
	SparseSet() {
		dense.reserve(MAX);
		sparse.reserve(MAX);
		resize_sparse(10);
	}

	template<typename ...Args>
	void add(uint32_t val, Args&&... args) {
		if (!val_valid(val) || !has_space()) {
			return;
		}
		dense.push_back(val);
		data_v.emplace_back(std::forward<Args>(args)...);
		if (sparse.size() < val) {
			//resize_sparse(k*2+1);
			resize_sparse(val * 2 + 1);
		}
		sparse[val] = count;
		count++;
	}

	const Data& get(uint32_t i) const {
		if (i >= count) {
			return INVALID;
		}
		return data_v[i];
	}

	bool has_val(uint32_t val) {
		if (!val_valid(val)) {
			return false;
		}
		auto i = sparse[val];
		return i >= count;
	}

	const std::vector<Data>& get_all() const {
		return data_v;
	}

private:
	bool val_valid(uint32_t val) const {
		return val < MAX && val != INVALID;
	}

	bool has_space() const {
		return count < MAX;
	}

	void resize_sparse(uint32_t r) {
		if (r <= sparse.size()) {
			return;
		}
		sparse.resize(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(r), INVALID);
	}

private:
	std::vector<uint32_t> dense;
	std::vector<Data> data_v;
	std::vector<uint32_t> sparse;
	uint32_t count = 0;
	uint32_t MAX = 1000;
	uint32_t INVALID = -1;
};


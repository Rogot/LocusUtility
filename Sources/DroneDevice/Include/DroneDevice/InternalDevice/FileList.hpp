//
// FileList.hpp
//
//  Created on: Jul 5, 2019
//      Author: Alexander
//

#ifndef DRONDEVICE_INTERNALDEVICE_FILELIST_HPP_
#define DRONDEVICE_INTERNALDEVICE_FILELIST_HPP_

#include <DroneDevice/InternalDevice/AbstractFile.hpp>
#include <tuple>
#include <type_traits>

namespace Device {

template<Device::FileId ID, typename FILE>
struct FileEntry {
	using Type = FILE;
	static constexpr Device::FileId kId{ID};

	FILE &entry;

	constexpr FileEntry(FILE &file) :
		entry{file}
	{
	}
};

template<typename... Files>
class FileList {
	std::tuple<Files...> entries;

public:
	constexpr FileList(Files... aFiles) :
		entries{std::tie(aFiles...)}
	{
	}

	constexpr Device::AbstractFile *get(Device::FileId aFile) const
	{
		return getImpl<0, Files...>(aFile);
	}

private:
	template<Device::FileId N, typename T>
	constexpr Device::AbstractFile *getImpl(Device::FileId aFile) const
	{
		return aFile == T::kId ? &std::get<N>(entries).entry : nullptr;
	}

	template<Device::FileId N, typename T1, typename T2, typename... Ts>
	constexpr Device::AbstractFile *getImpl(Device::FileId aFile) const
	{
		auto * const pointer = getImpl<N, T1>(aFile);
		return pointer != nullptr ? pointer : getImpl<N + 1, T2, Ts...>(aFile);
	}
};

template<Device::FileId N, typename T>
static constexpr auto makeFileEntry(T&& arg)
{
	return FileEntry<N, std::remove_reference_t<T>>{std::forward<T>(arg)};
}

template<typename... Ts>
static constexpr auto makeFileList(Ts&&... args)
{
	return FileList<std::remove_reference_t<Ts>...>(std::forward<Ts>(args)...);
}

} // namespace Device

#endif // DRONDEVICE_INTERNALDEVICE_FILELIST_HPP_

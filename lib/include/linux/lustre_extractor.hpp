#ifndef __SYSMAP_LUSTRE_EXTRACTOR_HPP__
#define __SYSMAP_LUSTRE_EXTRACTOR_HPP__

#include "../extractors/lustre_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Lustre_Extractor : extractor::Lustre_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Lustre_Extractor>(); }

        protected:
            virtual data collect() override;

        private:

            //TODO: get a better place
            enum mntdf_flags {
            	MNTDF_INODES	= 0x0001,
            	MNTDF_COOKED	= 0x0002,
            	MNTDF_LAZY	= 0x0004,
            	MNTDF_VERBOSE	= 0x0008,
            	MNTDF_SHOW	= 0x0010,
            };

            static Registrar registrar;
            int mntdf(char *mntdir, char *fsname,
		      int ops, struct ll_statfs_buf *lsb,
                      mountpoint *mntpoint);
            int getname(std::string mntdir, mountpoint *mntpoint);

            /**
             * Trys to create a file at the given mount_path + '/' + username.
             * If successful the striping information will be gathered and then
             * the file will be deleted again
             * @param mount_path Path to a Lustre Mountpoint
             * @param result container that will be filled with data
             * @return true on success
             */
            bool handle_getstripe(const std::string& mount_path, data& result);

            /**
             * Create a Dummy File using System Default stripe_size, stripe_count, ect
             *
             * @param path path to the new file to create
             * @return returns true if file was created without any error
             */
            bool create_dummy_file(const std::string &path) const;

            /**
             * Gets stripe size of the given file.
             *
             * @param path path to the file
             * @param result container that will be filled with data
             */
            bool file_getstripe(const std::string &path, data &result);

            void collect_mountpoint_data(data& result);
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_LUSTRE_EXTRACTOR_HPP__ */

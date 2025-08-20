outputDir="../Builds"
rulesDir="../Example/Rules"
headerDir="../Example"

if [ -n "$1" ]; then 
    outputDir="${outputDir}/$1"
fi

mkdir -p ${outputDir}

cp  -r "${rulesDir}" "${outputDir}/"

cd "../target"
for dir in *; do
    if [ -d "$dir" ] ; then
        echo $dir
        for file in $(find "$dir" -name "*mathcat_c.a" -o -name "*mathcat_c.so" -o -name "*mathcat_c.dll*" -o -name "*libmathcat_c.pdb");
        do
            mkdir "${outputDir}/${dir}"
            mkdir "${outputDir}/${dir}/include/"
            cp  "${headerDir}/mathcat.h" "${outputDir}/${dir}/include/"
            cp  "${headerDir}/mathcat-c.h" "${outputDir}/${dir}/include/"
            mkdir "${outputDir}/${dir}/lib/"
            cp  "$file" "${outputDir}/${dir}/lib/"
            echo $file
        done
    fi
done
cd -

cd ${outputDir}
for dir in *; do
    if [ -d "$dir" ] ; then
		if [ "${dir}" = "aarch64-linux-android" ] ; then
			mv "aarch64-linux-android" "mathcat-android-arm64"
		elif [ "$dir" = "armv7-linux-androideabi" ] ; then
			mv "armv7-linux-androideabi" "mathcat-android-arm"
		fi
    fi
done

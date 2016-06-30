## OpenCV C++ Stereo Camera Calibration

### Dependencies

- OpenCV

### Usage

#### Compilation

Compile all the files using the following commands:

```bash
mkdir build && cd build
cmake ..
make
```

#### Data

Some sample calibration images are stored in the `calib_imgs` folder.

#### Intrinsic Calibration of a single camera

This is only for lenses which follow the pinhole model. If you have fisheye lenses with a very wide field of view then see (this)[https://github.com/sourishg/fisheye_stereo_calibration] repository. The calibration saves the camera matrix and the distortion coefficients in a YAML file. The datatype for these matrices is `Mat`.

Once you have compiled the sources run the following command to calibrate the intrinsics. Make sure your are in the `build` folder.

```bash
./calibrate -w [board_width] -h [board_height] -n [num_imgs] -s [square_size] -d [imgs_directory] -i [imgs_filename] -o [extension] -e [output_filename]
```

For example, the command for the test images in `calib_imgs/1/` would be

```bash
./calibrate -w 9 -h 6 -n 27 -s 0.02423 -d "../calib_imgs/1/" -i "left" -o "cam_left.yml" -e "jpg"
```
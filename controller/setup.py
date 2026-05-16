from setuptools import find_packages, setup
import os
from glob import glob
package_name = 'controller'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
       ('share/ament_index/resource_index/packages',
	   ['resource/' + package_name]),
       ('share/' + package_name, ['package.xml']),
       ('share/' + package_name + '/launch', glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='evgenia',
    maintainer_email='evgeniakolovouu@gmail.com',
    description='Controller',
    license='Apache License 2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
                'controller_node = controller.controller_node:main',
        ],
    },
)

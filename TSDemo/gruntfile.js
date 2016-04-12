module.exports = function(grunt) {
	grunt.initConfig({
        clean: {
            build: ['./dist']
        },
        copy: {
            library: {
            	src: '../ODESolver/src/js-solver.js', dest: 'dist/lib/solver/js-solver.js'
            },
            dist: {
                files: [
                    {src: 'index.html', dest: 'dist/index.html'},
					{cwd: 'Styles', src: '*.css', dest: 'dist/Styles', expand: true},
					{cwd: 'Scripts/PrecompiledScripts', src: ['*.js', '*.d.ts'], dest: 'dist/Build/PrecompiledScripts', expand: true}
                ]
            }
        },
        bower: {
            install: {
                options: {
                    targetDir: './dist/lib'
                }
            }
        },
        ts: {
            dev: {
                src: ["Scripts/**/*.ts"],
                outDir: 'dist/Build',
                options: {
                    declaration: true,
                    sourceMap: true,
                    module: 'amd',
                }, 
            },
            release: {
                src: ["Scripts/**/*.ts"],
                outDir: 'dist/Build',
                options: {
                    declaration: false,
                    sourceMap: false,
                    module: 'amd',
                },                
            }
        },
        tsd: {
            refresh: {
                options: {
                    command: 'reinstall',
                    path: "typings",
                    latest: true,
                    config: 'tsd.json'
                }
            }
        },
        buildcontrol: {
            options: {
                dir: 'dist',
                commit: true,
                push: true,
                force: true,
                message: 'Built %sourceName% from commit %sourceCommit% on branch %sourceBranch%'
            },
            pages: {
                options: {
                    remote: 'git@github.com:Andrey1024/diplom.git',
                    branch: 'gh-pages'
                }
            },
        }
	});

	grunt.loadNpmTasks('grunt-bower-task');
	grunt.loadNpmTasks('grunt-contrib-clean');
	grunt.loadNpmTasks('grunt-contrib-copy');
	grunt.loadNpmTasks('grunt-tsd');
	grunt.loadNpmTasks('grunt-ts');
    grunt.loadNpmTasks('grunt-build-control');

	grunt.registerTask('build', ['copy:library', 'bower', 'ts:dev', 'copy:dist']);
    grunt.registerTask('installDeps', ['copy:library','bower', 'tsd']);
	grunt.registerTask('default', ['clean', 'build']);
	grunt.registerTask('deploy', ['clean', 'copy:library', 'bower', 'ts:release', 'copy:dist', 'buildcontrol']);
	

};

pipeline {
    agent any

    stages {
        stage('Update Git') {
            steps {
                script {
                    sh '''
                      git submodule init && git submodule update
                      cd mainui
                      git submodule init && git submodule update
                      cd ..
                    '''
                }
            }
        }

        stage('Configure') {
            steps {
                script {
                    sh 'python3 waf configure -T debug -8 -W --prefix=./build-artifacts'
                }
            }
        }

        stage('Build') {
            steps {
                script {
                    sh 'python3 waf build -j2'
                }
            }
        }

        stage('Archive Artifacts') {
            steps {
                script {
                    sh '''
                      ZIP_FILE="usource-${env.BRANCH_NAME}-$(uname)-$(uname -m)-latest.zip"
                      zip -r "$ZIP_FILE" ./*
                      mv "$ZIP_FILE" "/nfs/repo/public/dist/uSource/$BRANCH_NAME/$ZIP_FILE"
                    '''
                }
            }
        }

    }
}
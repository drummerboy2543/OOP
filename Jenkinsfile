pipeline {
    agent any
    stages {
        stage('build') {
            steps {
                sh 'echo hopefully everything builds'
            }
        }
    }
	post {
		success {
				sh 'echo We did it!'
		}
	}
}

package com.example.curtainopener

import androidx.lifecycle.ViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import okhttp3.Call
import okhttp3.Callback
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.Response
import java.io.IOException

data class GetStatusUiState(
    val status: String = ""
)

class HttpClientViewModel : ViewModel() {
    private val client = OkHttpClient()
    private val _uiState = MutableStateFlow(GetStatusUiState())
    val uiState: StateFlow<GetStatusUiState> = _uiState.asStateFlow()

    fun getStatus() {
        val request = Request.Builder().url("http://192.168.1.77:302/status").build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    _uiState.update { currentState ->
                        response.body?.let { it1 ->
                            currentState.copy(
                                status = it1.string()
                            )
                        }!!
                    }
                }
            }
        })
    }
}